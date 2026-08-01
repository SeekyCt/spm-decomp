#!/usr/bin/env python3

import struct
import sys
from pathlib import Path

from elf_symbols import globalize_and_rename_symbols


SYMBOLS = {
    "mot_walk.o": {
        (".sdata2", 0x00, 4): "lbl_805b2220",
        (".sdata2", 0x04, 4): "lbl_805b2224",
        (".sdata2", 0x08, 4): "lbl_805b2228",
        (".sdata2", 0x0c, 4): "lbl_805b222c",
        (".sdata2", 0x10, 4): "lbl_805b2230",
        (".sdata2", 0x14, 4): "lbl_805b2234",
        (".sdata2", 0x18, 4): "lbl_805b2238",
        (".sdata2", 0x1c, 4): "lbl_805b223c",
        (".rodata", 0x00, 8): "lbl_80333238",
    },
    "mario_move.o": {
        (".rodata", 0x00, 8): "lbl_80334430",
    },
}


def read_c_string(data: bytes, offset: int) -> str:
    return data[offset:data.index(0, offset)].decode("ascii")


def find_symbols(path: Path, wanted: dict[tuple[str, int, int], str]) -> dict[str, str]:
    data = path.read_bytes()
    if data[:6] != b"\x7fELF\x01\x02":
        raise ValueError("expected a big-endian ELF32 object")

    section_offset = struct.unpack_from(">I", data, 0x20)[0]
    section_size = struct.unpack_from(">H", data, 0x2e)[0]
    section_count = struct.unpack_from(">H", data, 0x30)[0]
    section_string_index = struct.unpack_from(">H", data, 0x32)[0]
    section_string_header = section_offset + section_string_index * section_size
    section_string_offset = struct.unpack_from(">I", data, section_string_header + 0x10)[0]

    sections = []
    for index in range(section_count):
        header = section_offset + index * section_size
        name_offset, section_type = struct.unpack_from(">II", data, header)
        sections.append({
            "index": index,
            "name": read_c_string(data, section_string_offset + name_offset),
            "type": section_type,
            "offset": struct.unpack_from(">I", data, header + 0x10)[0],
            "size": struct.unpack_from(">I", data, header + 0x14)[0],
            "link": struct.unpack_from(">I", data, header + 0x18)[0],
            "entry_size": struct.unpack_from(">I", data, header + 0x24)[0],
        })

    symtab = next(section for section in sections if section["type"] == 2)
    strings = sections[symtab["link"]]
    found = {}
    for offset in range(0, symtab["size"], symtab["entry_size"]):
        entry = symtab["offset"] + offset
        name_offset, value, size = struct.unpack_from(">III", data, entry)
        section_index = struct.unpack_from(">H", data, entry + 0xe)[0]
        if section_index >= len(sections) or name_offset == 0:
            continue
        key = (sections[section_index]["name"], value, size)
        if key in wanted:
            source = read_c_string(data, strings["offset"] + name_offset)
            if key in found:
                raise ValueError(f"multiple symbols found for {key}")
            found[key] = source

    missing = set(wanted) - set(found)
    if missing:
        raise ValueError(f"symbols not found at expected locations: {sorted(missing)}")
    return {found[key]: target for key, target in wanted.items()}


def main() -> None:
    path = Path(sys.argv[1])
    wanted = SYMBOLS.get(path.name)
    if wanted is None:
        raise ValueError(f"no symbol mapping for {path.name}")
    mappings = find_symbols(path, wanted)

    globalize_and_rename_symbols(path, mappings)


if __name__ == "__main__":
    main()
