"""
Creates a context of all headers with the preprocessor ran
"""

from argparse import ArgumentParser
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


def preprocess(cc: str, includes: str, defines: List[str]) -> str:
    """Gets the preprocessed text of a C file"""

    # Fallback to default defines
    defines_str = ' '.join(f"-d {define}" for define in defines)

    # Run mwcc preprocessor
    with NamedTemporaryFile(suffix=".c", delete=False) as tmp:
        try:
            tmp.write(includes.encode())
            tmp.close()
            # NOTE: not preprocessed in C++ mode
            return c.get_cmd_stdout(
                f"{cc} -I- {c.MWCC_INCLUDES} {defines_str} -d M2C -stderr -E {tmp.name}"
            )
        finally:
            unlink(tmp.name)


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("-d", "--defines", type=str, nargs='+', default=c.DEFINES,
                        help="Preprocessor defines")
    parser.add_argument("-w", "--wine", type=str, help="Wine override (ignored on Windows)")
    args = parser.parse_args()

    # Get cc command
    cc = c.check_wine(c.CC, args.wine)

    # Find all headers
    includes = make_includes(c.INCDIRS)

    # Preprocess headers
    out = preprocess(cc, includes, args.defines)

    # Output
    print(out)
