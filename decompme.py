"""
Creates a decomp.me scratch for a function
"""

from argparse import ArgumentParser

import requests

import common as c
from getfunction import get_function

# Get arguments
parser = ArgumentParser()
hex_int = lambda s: int(s, 16)
parser.add_argument("addr", type=hex_int)
parser.add_argument("--host", default="https://decomp.me")
args = parser.parse_args()

# Find containing binary
binary, source = c.get_containing_slice(args.addr)

# Get flags for binary
if binary == c.Binary.DOL:
    preset = "Super Paper Mario (DOL)"
    flags = c.EXTERNAL_DOL_CFLAGS
else:
    preset = "Super Paper Mario (REL)"
    flags = c.EXTERNAL_REL_CFLAGS

# Disassemble function
srcflag = f"-n {source}" if isinstance(source, str) else ""
asm = get_function(binary, srcflag, args.addr, True)

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
