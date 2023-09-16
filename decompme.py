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
parser.add_argument("-n", "--source-name", type=str, help="Prioritise source-local symbols")
parser.add_argument("--host", default="https://decomp.me")
parser.add_argument("-w", "--wine", type=str, help="Wine override (ignored on Windows)")
args = parser.parse_args()

# Find address and diff_label
assert not (args.dol and args.rel), "--dol and --rel are incompatible"
addr, diff_label = c.lookup_sym_full(args.sym, args.dol, args.rel, args.source_name)
assert addr is not None, f"Symbol {args.sym} not found"
if diff_label is None:
    diff_label = f"func_{addr:x}"

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
asm = get_function(binary, source, addr, True)

# Get context
if args.wine:
    wine = f"--wine {args.wine}"
else:
    wine = ""
ctx = c.get_cmd_stdout(f"{c.PYTHON} makectx_m2c.py {wine}")

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
