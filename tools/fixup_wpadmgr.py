#!/usr/bin/env python3

import struct
import sys
from pathlib import Path


def find_section(data: bytes, name: bytes) -> tuple[int, int]:
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 2:
        raise ValueError("expected a big-endian ELF32 object")

    section_offset = struct.unpack_from(">I", data, 0x20)[0]
    section_size = struct.unpack_from(">H", data, 0x2e)[0]
    section_count = struct.unpack_from(">H", data, 0x30)[0]
    string_index = struct.unpack_from(">H", data, 0x32)[0]
    string_header = section_offset + string_index * section_size
    string_offset = struct.unpack_from(">I", data, string_header + 0x10)[0]

    for index in range(section_count):
        header = section_offset + index * section_size
        name_offset = struct.unpack_from(">I", data, header)[0]
        end = data.index(0, string_offset + name_offset)
        if data[string_offset + name_offset:end] == name:
            return struct.unpack_from(">II", data, header + 0x10)
    raise ValueError(f"section {name.decode()} not found")


def main() -> None:
    path = Path(sys.argv[1])
    data = bytearray(path.read_bytes())
    text_offset, text_size = find_section(data, b".text")

    patches = {
        0x78c: (bytes.fromhex("38810100"), bytes.fromhex("38c10010")),
        0x79c: (bytes.fromhex("38c10010"), bytes.fromhex("38810100")),
        0x8d8: (bytes.fromhex("38810100"), bytes.fromhex("38a10010")),
        0x8dc: (bytes.fromhex("38a10010"), bytes.fromhex("38810100")),
    }
    if text_size < max(patches) + 4:
        raise ValueError("wpadmgr .text is smaller than expected")

    for offset, (expected, replacement) in patches.items():
        start = text_offset + offset
        actual = bytes(data[start:start + 4])
        if actual != expected:
            raise ValueError(
                f"unexpected instruction at .text+0x{offset:x}: "
                f"expected {expected.hex()}, found {actual.hex()}"
            )
        data[start:start + 4] = replacement

    path.write_bytes(data)


if __name__ == "__main__":
    main()
