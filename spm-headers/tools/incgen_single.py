"""
Creates a source file including all headers
"""

from argparse import ArgumentParser
from random import seed, shuffle


def make_include(path: str) -> str:
    """Generates a #include for a path"""

    return f"#include <{path}>"


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("path", type=str, help="Header to include")
    args = parser.parse_args()

    # Find all headers
    includes = make_include(args.path)

    # Output
    print(includes)
