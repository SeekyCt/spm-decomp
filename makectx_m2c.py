"""
Creates a context of all headers with the preprocessor ran
"""

from os import listdir, unlink
from os.path import isdir
from tempfile import NamedTemporaryFile
from typing import List

import common as c

def find_headers(dirname: str, base=None) -> List[str]:
    """Returns a list of all headers in a folder recursively"""

    if base is None:
        base = dirname

    ret = []
    for name in listdir(dirname):
        path = dirname + '/' + name
        if isdir(path):
            ret.extend(find_headers(path, base))
        elif name.endswith('.h'):
            ret.append(path[len(base)+1:])

    return ret

def make_includes(dirname: str) -> str:
    """Returns a chain of #includes for all headers in a folder"""

    return '\n'.join([
        f"#include <{header}>"
        for header in find_headers(dirname)
    ])

# Find all headers
includes = make_includes("include")

# Run mwcc preprocessor
with NamedTemporaryFile(suffix=".c", delete=False) as tmp:
    try:
        tmp.write(includes.encode())
        tmp.close()
        out = c.get_cmd_stdout(f"{c.CC} -I- -i include -DM2C -stderr -E {tmp.name}")
    finally:
        unlink(tmp.name)

# Output
print(out)
