"""
Disassembles a single function
"""

from argparse import ArgumentParser
from os import system, unlink
import os.path
from tempfile import NamedTemporaryFile

import common as c

def get_function(binary: c.Binary, srcflag: str, addr: int, extra: bool) -> str:
    # Get context
    ctx = c.DOL_CTX if binary == c.Binary.DOL else c.REL_CTX
    assert os.path.exists(ctx.labels), "Error: analysis has not ran!"

    # Disassemble function
    extraflag = "-e" if extra else ""
    with NamedTemporaryFile(suffix=".s", delete=False) as tmp:
        try:
            tmp.close()
            ret = system(
                f"{c.DISASSEMBLER} {ctx.binary} {ctx.labels} {ctx.relocs} {tmp.name} -f {addr:x} "
                f"-m {c.SYMBOLS} {srcflag} -q {extraflag}"
            )
            assert ret == 0, f"Disassembly error code {ret}"
            with open(tmp.name) as f:
                asm = f.read()
        finally:
            unlink(tmp.name)
    
    return asm

if __name__=="__main__":
    parser = ArgumentParser()
    hex_int = lambda s: int(s, 16)
    parser.add_argument("addr", type=hex_int)
    parser.add_argument("-e", "--extra", action="store_true",
                        help="For --function, include referenced jumptables")
    args = parser.parse_args()

    # Find containing binary
    binary, source = c.get_containing_slice(args.addr)

    # Get source file name flag
    srcflag = f"-n {source}" if isinstance(source, str) else ""

    print(get_function(binary, srcflag, args.addr, args.extra))
