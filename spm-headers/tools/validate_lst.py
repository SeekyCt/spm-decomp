"""
Simple tool to check rel addresses aren't listed as absolute
"""

from dataclasses import dataclass
import re
from sys import argv


@dataclass
class DolSymbol:
    """Container for a symbol in a dol file"""

    addr: int
    name: str


@dataclass
class RelSymbol:
    """Container for a symbol in a rel file"""

    module_id: int
    section_id: int
    offset: int
    name: str


def load_lst(filename: str) -> tuple[list[DolSymbol], list[RelSymbol]]:
    """Parses an LST symbol map"""

    # Load LST
    with open(filename) as f:
        lines = f.readlines()

    # Parse lines
    dol_symbols = []
    rel_symbols = []
    for i, line in enumerate(lines):
        # Ignore comments and whitespace
        line = line.strip()
        if line.startswith("/") or len(line) == 0:
            continue

        # Try parse
        try:
            # Dol - addr:name
            # Rel - moduleId,sectionId,offset:name
            colon_parts = [s.strip() for s in line.split(":")]
            other, name = colon_parts
            comma_parts = [s.strip() for s in other.split(',')]
            if len(comma_parts) == 1:
                addr, = comma_parts
                dol_symbols.append(
                    DolSymbol(
                        int(addr, 16),
                        name
                    )
                )
            else:
                module_id, section_id, offset = comma_parts
                rel_symbols.append(
                    RelSymbol(
                        int(module_id, 0),
                        int(section_id, 0),
                        int(offset, 16),
                        name
                    )
                )
                # ignore rel symbols
        except Exception as e:
            raise Exception(f"Error on line {i+1}: {e}")

    return dol_symbols, rel_symbols


def find_absolute_rel_syms(path: str) -> list[DolSymbol]:
    dol_syms, _ = load_lst(path)

    ret = []
    for sym in dol_syms:
        if sym.addr > 0x80c0_0000:
            ret.append(sym)

    return ret


if __name__ == "__main__":
    errors = find_absolute_rel_syms(argv[1])
    if len(errors) > 0:
        print("Invalid lst", argv[1])
        for sym in errors:
            print(f"Absolute address used for rel symbol {sym.name} ({sym.addr:x})")
        exit(1)
