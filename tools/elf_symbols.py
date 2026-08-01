#!/usr/bin/env python3

import math
import struct
from pathlib import Path


SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_RELA = 4
SHT_REL = 9
SHT_GROUP = 17
SHT_SYMTAB_SHNDX = 18
STB_LOCAL = 0
STB_GLOBAL = 1


def _read_string(data: bytes | bytearray, offset: int) -> bytes:
    return bytes(data[offset:data.index(0, offset)])


def globalize_and_rename_symbols(path: Path, mappings: dict[str, str]) -> None:
    """Globalize and rename local symbols in a big-endian ELF32 object."""
    if not mappings or len(set(mappings.values())) != len(mappings):
        raise ValueError("symbol mappings must have unique sources and targets")

    data = bytearray(path.read_bytes())
    if data[:6] != b"\x7fELF\x01\x02":
        raise ValueError("expected a big-endian ELF32 object")
    if struct.unpack_from(">HH", data, 0x10) != (1, 20):
        raise ValueError("expected a relocatable PowerPC ELF object")
    if struct.unpack_from(">H", data, 0x2c)[0] != 0:
        raise ValueError("ELF objects with program headers are not supported")

    section_offset = struct.unpack_from(">I", data, 0x20)[0]
    section_size = struct.unpack_from(">H", data, 0x2e)[0]
    section_count = struct.unpack_from(">H", data, 0x30)[0]
    if section_size != 0x28 or section_count == 0:
        raise ValueError("expected ordinary 40-byte ELF32 section headers")

    sections = []
    for index in range(section_count):
        header = section_offset + index * section_size
        values = struct.unpack_from(">IIIIIIIIII", data, header)
        sections.append({
            "index": index,
            "type": values[1],
            "offset": values[4],
            "size": values[5],
            "link": values[6],
            "info": values[7],
            "alignment": values[8],
            "entry_size": values[9],
        })

    symbol_tables = [section for section in sections if section["type"] == SHT_SYMTAB]
    if len(symbol_tables) != 1:
        raise ValueError("expected exactly one ELF symbol table")
    symbols = symbol_tables[0]
    if symbols["entry_size"] != 0x10 or symbols["size"] % 0x10 != 0:
        raise ValueError("expected 16-byte ELF32 symbols")
    if symbols["link"] >= section_count:
        raise ValueError("symbol string table index is out of range")
    strings = sections[symbols["link"]]
    if strings["type"] != SHT_STRTAB:
        raise ValueError("symbol table does not link to a string table")

    entries = []
    names: dict[bytes, list[int]] = {}
    for index in range(symbols["size"] // 0x10):
        start = symbols["offset"] + index * 0x10
        entry = bytes(data[start:start + 0x10])
        name_offset = struct.unpack_from(">I", entry)[0]
        name = _read_string(data, strings["offset"] + name_offset)
        entries.append(entry)
        names.setdefault(name, []).append(index)

    first_nonlocal = symbols["info"]
    if not 0 < first_nonlocal <= len(entries):
        raise ValueError("invalid first non-local symbol index")
    if any(entry[0xc] >> 4 != STB_LOCAL for entry in entries[:first_nonlocal]):
        raise ValueError("non-local symbol appears in the local symbol range")
    if any(entry[0xc] >> 4 == STB_LOCAL for entry in entries[first_nonlocal:]):
        raise ValueError("local symbol appears after the local symbol range")

    encoded_mappings = {
        source.encode("ascii"): target.encode("ascii")
        for source, target in mappings.items()
    }
    duplicate_targets = set(encoded_mappings.values()) & set(names)
    if duplicate_targets:
        formatted = ", ".join(sorted(name.decode("ascii") for name in duplicate_targets))
        raise ValueError(f"target symbols already exist: {formatted}")

    mapped_indices = {}
    for source in encoded_mappings:
        indices = names.get(source, [])
        if len(indices) != 1:
            raise ValueError(
                f"expected exactly one symbol named {source.decode('ascii')}, found {len(indices)}"
            )
        index = indices[0]
        if entries[index][0xc] >> 4 != STB_LOCAL:
            raise ValueError(f"symbol {source.decode('ascii')} is not local")
        mapped_indices[index] = source

    appended_strings = bytearray()
    target_name_offsets = {}
    for source, target in encoded_mappings.items():
        target_name_offsets[source] = strings["size"] + len(appended_strings)
        appended_strings.extend(target)
        appended_strings.append(0)

    local_indices = []
    nonlocal_indices = []
    for index, entry in enumerate(entries):
        if index in mapped_indices:
            continue
        if entry[0xc] >> 4 == STB_LOCAL:
            local_indices.append(index)
        else:
            nonlocal_indices.append(index)
    order = local_indices + list(mapped_indices) + nonlocal_indices
    if len(order) != len(entries) or sorted(order) != list(range(len(entries))):
        raise ValueError("failed to reorder the ELF symbol table")
    index_map = {old: new for new, old in enumerate(order)}

    reordered = bytearray()
    for old_index in order:
        entry = bytearray(entries[old_index])
        source = mapped_indices.get(old_index)
        if source is not None:
            struct.pack_into(">I", entry, 0, target_name_offsets[source])
            entry[0xc] = (STB_GLOBAL << 4) | (entry[0xc] & 0x0f)
        reordered.extend(entry)
    symbol_start = symbols["offset"]
    data[symbol_start:symbol_start + symbols["size"]] = reordered

    group_info = {}
    for section in sections:
        if section["link"] != symbols["index"]:
            continue
        if section["type"] in (SHT_REL, SHT_RELA):
            expected_size = 8 if section["type"] == SHT_REL else 12
            if section["entry_size"] != expected_size or section["size"] % expected_size:
                raise ValueError("unexpected ELF relocation entry size")
            for relative in range(0, section["size"], expected_size):
                info_offset = section["offset"] + relative + 4
                info = struct.unpack_from(">I", data, info_offset)[0]
                old_index = info >> 8
                if old_index not in index_map:
                    raise ValueError("relocation symbol index is out of range")
                new_info = (index_map[old_index] << 8) | (info & 0xff)
                struct.pack_into(">I", data, info_offset, new_info)
        elif section["type"] == SHT_GROUP:
            if section["info"] not in index_map:
                raise ValueError("group signature symbol index is out of range")
            group_info[section["index"]] = index_map[section["info"]]
        elif section["type"] == SHT_SYMTAB_SHNDX:
            if section["entry_size"] not in (0, 4) or section["size"] != len(entries) * 4:
                raise ValueError("unexpected extended symbol index table")
            old_values = struct.unpack_from(f">{len(entries)}I", data, section["offset"])
            new_values = [old_values[old_index] for old_index in order]
            struct.pack_into(f">{len(entries)}I", data, section["offset"], *new_values)

    insert_offset = strings["offset"] + strings["size"]
    alignment = 4
    for section in sections:
        if section["offset"] >= insert_offset:
            alignment = math.lcm(alignment, max(section["alignment"], 1))
    padding = (-len(appended_strings)) % alignment
    appended_strings.extend(b"\0" * padding)
    if not appended_strings:
        raise ValueError("no symbol names were supplied")
    growth = len(appended_strings)
    data[insert_offset:insert_offset] = appended_strings

    new_section_offset = section_offset + growth if section_offset >= insert_offset else section_offset
    struct.pack_into(">I", data, 0x20, new_section_offset)
    for section in sections:
        header = new_section_offset + section["index"] * section_size
        old_offset = section["offset"]
        if old_offset and old_offset >= insert_offset:
            struct.pack_into(">I", data, header + 0x10, old_offset + growth)
        if section["index"] == strings["index"]:
            struct.pack_into(">I", data, header + 0x14, section["size"] + growth)
        if section["index"] == symbols["index"]:
            struct.pack_into(">I", data, header + 0x1c, len(local_indices))
        if section["index"] in group_info:
            struct.pack_into(">I", data, header + 0x1c, group_info[section["index"]])

    path.write_bytes(data)
