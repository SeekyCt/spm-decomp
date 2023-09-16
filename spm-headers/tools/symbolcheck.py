from argparse import ArgumentParser
from dataclasses import dataclass
from typing import Dict, Set

from pycparser.c_parser import CParser
from pycparser import c_ast as ca


class Visitor(ca.NodeVisitor):
    decls: Set[str]

    def __init__(self):
        self.decls = set()

    def visit_Struct(self, struct: ca.Struct) -> None:
        pass

    def visit_Union(self, union: ca.Union) -> None:
        pass

    def visit_Decl(self, decl: ca.Decl) -> None:
        if decl.name is not None:
            self.decls.add(decl.name)
        if not isinstance(decl.type, ca.FuncDecl):
            self.visit(decl.type)

    def visit_Enum(self, enum: ca.Enum) -> None:
        pass

    def visit_FuncDef(self, fn: ca.FuncDef) -> None:
        if fn.decl.name is not None:
            self.decls.add(fn.decl.name)


@dataclass(frozen=True)
class SymbolLoc:
    module_id: int
    section_id: int
    offset: int


class LstParser:
    name_map: Dict[str, SymbolLoc]
    loc_map: Dict[SymbolLoc, str]

    def _parse_line(self, line: str):
        line = line.lstrip()
        if len(line) == 0 or line.startswith("//"):
            return

        colon_parts = [s.strip() for s in line.split(":")]
        if len(colon_parts) != 2:
            return
        name = colon_parts[1]

        comma_parts = colon_parts[0].split(",")
        if len(comma_parts) == 1:
            module_id = 0
            section_id = 0
            offset = int(comma_parts[0], 16)
        else:
            module_id = int(comma_parts[0])
            section_id = int(comma_parts[1])
            offset = int(comma_parts[2], 16)

        loc = SymbolLoc(module_id, section_id, offset)
        self.name_map[name] = loc
        self.addr_map[loc] = name

    def __init__(self, path: str):
        self.name_map = {}
        self.addr_map = {}

        with open(path) as f:
            lines = f.readlines()

        for line in lines:
            self._parse_line(line)

    def get_name(self, loc: SymbolLoc) -> str:
        return self.loc_map[loc]

    def get_loc(self, name: SymbolLoc) -> str:
        return self.addr_map[name]

    def get_names(self) -> Set[str]:
        return set(self.name_map.keys())


def get_c_decls(path: str) -> Set[str]:
    with open(path, "r") as f:
        txt = f.read()

    c_parser = CParser()
    ast = c_parser.parse(txt)
    v = Visitor()
    v.visit(ast)
    return v.decls


def get_lst_decls(path: str) -> Set[str]:
    lst_decls = LstParser(args.lst_path)
    return lst_decls.get_names()


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("ctx_path", type=str, help="Context path")
    parser.add_argument("lst_path", type=str, help="LST path")
    args = parser.parse_args()

    ctx_decls = get_c_decls(args.ctx_path)
    lst_decls = get_lst_decls(args.lst_path)

    extra = lst_decls - ctx_decls
    missing = ctx_decls - lst_decls
    print("\n".join(f"+ {s}" for s in sorted(extra)))
    print("\n".join(f"- {s}" for s in sorted(missing)))
