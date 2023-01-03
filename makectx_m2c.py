"""
Creates a context of all headers with the preprocessor ran
"""

from os import unlink
from tempfile import NamedTemporaryFile
from typing import List

import common as c

def make_includes(dirnames: List[str]) -> str:
    """Returns a chain of #includes for all headers in a folder"""

    return '\n'.join(
        '\n'.join(
            f"#include <{header}>"
            for header in c.find_headers(dirname)
        )
        for dirname in dirnames
    )

# Find all headers
includes = make_includes(c.INCDIRS)

# Run mwcc preprocessor
with NamedTemporaryFile(suffix=".c", delete=False) as tmp:
    try:
        tmp.write(includes.encode())
        tmp.close()
        out = c.get_cmd_stdout(
            f"{c.CC} -I- {c.MWCC_INCLUDES} {c.MWCC_DEFINES} -d M2C -stderr -E {tmp.name}"
        )
    finally:
        unlink(tmp.name)

# Output
print(out)
