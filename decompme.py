"""
Creates a decomp.me scratch for a function
"""

from argparse import ArgumentParser

import requests

import common as c
from getfunction import get_function

# Get arguments
parser = ArgumentParser()
parser.add_argument("sym", type=str, help="Symbol name or address")
parser.add_argument("-d", "--dol", action="store_true", help="Prioritise dol-local symbols")
parser.add_argument("-r", "--rel", action="store_true", help="Prioritise rel-local symbols")
parser.add_argument("-n", "--source-name", action="store_true",
                    help="Source C/C++ file name for symbol lookup")
parser.add_argument("--host", default="https://decomp.me")
args = parser.parse_args()

# Find address
assert not (args.dol and args.rel), "--dol and --rel are incompatible"
addr = c.lookup_sym(args.sym, args.dol, args.rel, args.source_name)

# Find containing binary
binary, source = c.get_containing_slice(addr)

# Get flags for binary
if binary == c.Binary.DOL:
    preset = "Super Paper Mario (DOL)"
    flags = c.EXTERNAL_DOL_CFLAGS
else:
    preset = "Super Paper Mario (REL)"
    flags = c.EXTERNAL_REL_CFLAGS

# Disassemble function
srcflag = f"-n {source}" if isinstance(source, str) else ""
asm = get_function(binary, srcflag, addr, True)

# Get diff_label
lines = asm.splitlines()
assert lines[0].startswith(".global "), "Address doesn't appear to be a function"
diff_label = lines[0][len(".global "):]

# Get context
ctx = c.get_cmd_stdout(f"{c.PYTHON} makectx_m2c.py")

# Send request
req = {
    "target_asm" : asm,
    "context" : ctx,
    "platform" : "gc_wii",
    "compiler" : "mwcc_41_60831",
    "compiler_flags" : flags,
    "preset" : preset,
    "diff_label" : diff_label
}
r = requests.post(args.host + "/api/scratch", json=req)
assert r.status_code == 201, f"Bad status code {r.status_code}"
print(args.host + r.json()["html_url"])
