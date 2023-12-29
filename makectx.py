"""
Creates a context of all headers
"""

import os.path
from typing import List

import common as c

gIncluded = set()

def resolve_path(path: str, bases: List[str]) -> str:
    ret = None
    for base in bases:
        full = f"{base}/{path}"
        if os.path.exists(full):
            assert ret is None, f"Name conflict: {full} {ret}"
            ret = full
    assert ret is not None, f"Header {path} not found"
    return ret

def process_header(path: str, bases: List[str]):
    global gIncluded

    if path in gIncluded:
        return ""
    gIncluded.add(path)

    full = resolve_path(path, bases)

    ret = ""
    with open(full) as f:
        for line in f:
            if line.startswith("#include") and not "makectx-ignore" in line:
                ret += process_header(line[line.find('<')+1:line.find('>')], bases)
            elif not line.startswith("#pragma once"):
                ret += line

    return ret

def makectx(bases: List[str]):
    ret = ""
    for base in bases:
        for header in c.find_headers(base):
            ret += process_header(header, bases)
    return ret

print(makectx(c.INCDIRS))
