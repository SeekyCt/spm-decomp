"""
Creates a context of all headers
"""

from os import listdir
from os.path import isdir

gIncluded = set()

def process_header(path, base):
    global gIncluded

    if path in gIncluded:
        return ""

    ret = ""
    with open(path) as f:
        for line in f:
            if line.startswith("#include"):
                ret += process_header(base + '/' + line[line.find('<')+1:line.find('>')], base)
            elif not line.startswith("#pragma once"):
                ret += line
    
    gIncluded.add(path)

    return ret

def concat_headers(dirname, base=None):
    if base is None:
        base = dirname
    ret = ""
    for name in listdir(dirname):
        path = dirname + '/' + name
        if isdir(path):
            ret += concat_headers(path, base)
        elif name.endswith('.h'):
            ret += (process_header(path, base))
    return ret
            
print(concat_headers("include"))
