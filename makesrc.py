"""
Makes a source file skeleton from a slice
"""

from argparse import ArgumentParser
import os
from os import system

import common as c

def make_src(path: str, data=False):
    # Get context
    ctx = c.DOL_CTX if path.startswith("src/") else c.REL_CTX
    assert os.path.exists(ctx.labels), "Error: analysis has not ran!"

    # Disassemble function
    dataflag = "-d" if data else ""
    ret = system(
        f"{c.MAKESRC} {ctx.binary} {ctx.labels} {ctx.relocs} {ctx.slices} {path} -p {ctx.srcdir}/ "
        f"{dataflag} -q"
    )
    assert ret == 0, f"Disassembly error code {ret}"

if __name__=="__main__":
    parser = ArgumentParser()
    parser.add_argument("path", type=str, help="Source file path")
    parser.add_argument("-d", "--data", action="store_true", help="Include data")
    args = parser.parse_args()
    make_src(args.path, args.data)
