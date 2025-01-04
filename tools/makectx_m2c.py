"""
Creates a context of all headers with the preprocessor ran
"""

from argparse import ArgumentParser
import os
import subprocess
import sys
from tempfile import NamedTemporaryFile
from typing import List, Optional


SPM_HEADERS = "spm-headers"

INCLUDES = [
    os.path.join(SPM_HEADERS, "include"),
    os.path.join(SPM_HEADERS, "decomp"),
    # include_cpp omitted for m2c compatability
]
CC = os.path.join("build", "compilers", "GC", "3.0a5.2", "mwcceppc")
DEFINES = [
    "DECOMP",
    "SPM_EU0"
]


def check_wine(command: str, wine_override: Optional[str] = None) -> str:
    if wine_override is None:
        wine_override = "wine"
    
    if sys.platform != "win32":
        return f"{wine_override} {command}"
    else:
        return command


def get_cmd_stdout(cmd: str, text=True) -> str:
    """Run a command and get the stdout output as a string"""

    ret = subprocess.run(cmd.split(), stdout=subprocess.PIPE, text=text)
    assert ret.returncode == 0, f"Command '{cmd}' returned {ret.returncode}"
    return ret.stdout


def find_headers(dirname: str, base=None) -> List[str]:
    """Returns a list of all headers in a folder recursively"""

    if base is None:
        base = dirname

    ret = []
    for name in os.listdir(dirname):
        path = dirname + '/' + name
        if os.path.isdir(path):
            ret.extend(find_headers(path, base))
        else:
            _, ext = os.path.splitext(path)
            if ext == ".h":
                ret.append(path[len(base)+1:])
            elif ext in (".hpp", ""):
                # C++ not supported
                pass
            elif ext in (".md",):
                # Ignore docs
                pass
            else:
                print("Warning: skipping unknown file type", name, file=sys.stderr)

    return ret


def make_includes(dirnames: List[str]) -> str:
    """Returns a chain of #includes for all headers in a folder"""

    return '\n'.join(
        '\n'.join(
            f"#include <{header}>"
            for header in find_headers(dirname)
        )
        for dirname in dirnames
    )


def preprocess(cc: str, includes: str, defines: List[str]) -> str:
    """Gets the preprocessed text of a C file"""

    defines_str = ' '.join(f"-d {define}" for define in defines)
    includes_str = ' '.join(f"-i {include}" for include in INCLUDES)

    # Run mwcc preprocessor
    with NamedTemporaryFile(suffix=".c", delete=False) as tmp:
        try:
            tmp.write(includes.encode())
            tmp.close()

            # NOTE: not preprocessed in C++ mode
            return get_cmd_stdout(
                f"{cc} -I- {includes_str} {defines_str} -d M2C -stderr -E {tmp.name}"
            )
        finally:
            os.unlink(tmp.name)


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("-d", "--defines", type=str, nargs='+', default=DEFINES,
                        help="Preprocessor defines")
    parser.add_argument("-w", "--wine", type=str, help="Wine override (ignored on Windows)")
    parser.add_argument("-o", "--out", type=str, help="Output file path")
    args = parser.parse_args()

    # Get cc command
    cc = check_wine(CC, args.wine)

    # Find all headers
    includes = make_includes(INCLUDES)

    # Preprocess headers
    out = preprocess(cc, includes, args.defines)

    # Output
    print(out)
