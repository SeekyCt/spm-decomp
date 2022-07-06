"""
Common functions & definitions
"""

from enum import Enum
from hashlib import sha1
import json
import os
from subprocess import PIPE, Popen
from sys import executable as PYTHON, platform
from typing import List, Tuple, Union

#############
# Functions #
#############

def get_file_sha1(path: str) -> bytes:
    """Gets the SHA1 hash of a file"""

    with open(path, 'rb') as f:
        return sha1(f.read()).digest()

def get_cmd_stdout(cmd: str, text=True) -> str:
    """Run a command and get the stdout output as a string"""

    proc = Popen(cmd.split(), stdout=PIPE, text=text)
    ret = proc.stdout.read()
    proc.wait()
    assert proc.returncode == 0, f"Command '{cmd}' returned {proc.returncode}"
    return ret

class Binary(Enum):
    DOL = 1
    REL = 2

# ppcdis source output
SourceDesc = Union[str, Tuple[str, int, int]]

def get_containing_slice(addr: int) -> Tuple[Binary, SourceDesc]:
    """Finds the binary containing an address and its source file
    Source file is empty string if not decompiled"""

    dol_raw = get_cmd_stdout(f"{SLICES} {DOL} {DOL_SLICES} -p {DOL_SRCDIR}/ --containing {addr:x}")
    containing = json.loads(dol_raw)
    if containing is None:
        rel_raw = get_cmd_stdout(
            f"{SLICES} {REL} {REL_SLICES} {PPCDIS_REL_FLAGS} -p {REL_SRCDIR}/ --containing {addr:x}"
        )
        containing = json.loads(rel_raw)
        assert containing is not None, f"Unknown address {addr:x}"
        return (Binary.REL, containing)
    else:
        return (Binary.DOL, containing)

def find_headers(dirname: str, base=None) -> List[str]:
    """Returns a list of all headers in a folder recursively"""

    if base is None:
        base = dirname

    ret = []
    for name in os.listdir(dirname):
        path = dirname + '/' + name
        if os.path.isdir(path):
            ret.extend(find_headers(path, base))
        elif name.endswith('.h'):
            ret.append(path[len(base)+1:])

    return ret

################
# Project dirs #
################

# Directory for decompiled dol code
DOL_SRCDIR = "src"

# Directory for decompiled rel code
REL_SRCDIR = "rel"

# Include directory
INCDIR = "include"

# Build artifacts directory
BUILDDIR = "build"

# Build include directory
BUILD_INCDIR = f"{BUILDDIR}/include"

# Output binaries directory
OUTDIR = "out"

# Original binaries directory
ORIG = "orig"

# Tools directory
TOOLS = "tools"

# Config directory
CONFIG = "config"

#########
# Tools #
#########

# ppcdis
PPCDIS = "tools/ppcdis"
PPCDIS_INCDIR = f"{PPCDIS}/include"
ANALYSER = f"{PYTHON} {PPCDIS}/analyser.py"
DISASSEMBLER = f"{PYTHON} {PPCDIS}/disassembler.py"
ORDERSTRINGS = f"{PYTHON} {PPCDIS}/orderstrings.py"
ORDERFLOATS = f"{PYTHON} {PPCDIS}/orderfloats.py"
FORCEACTIVE = f"{PYTHON} {PPCDIS}/forceactive.py"
ELF2DOL = f"{PYTHON} {PPCDIS}/elf2dol.py"
ELF2REL = f"{PYTHON} {PPCDIS}/elf2rel.py"
SLICES = f"{PYTHON} {PPCDIS}/slices.py"

# Codewarrior
TOOLS = "tools"
CODEWARRIOR = os.path.join(TOOLS, "4199_60831")
CC = os.path.join(CODEWARRIOR, "mwcceppc")
LD = os.path.join(CODEWARRIOR, "mwldeppc")
if platform != "win32":
    CC = f"wine {CC}"
    LD = f"wine {LD}"

# DevkitPPC
DEVKITPPC = os.environ.get("DEVKITPPC")
AS = os.path.join(DEVKITPPC, "bin", "powerpc-eabi-as")
OBJDUMP = os.path.join(DEVKITPPC, "bin", "powerpc-eabi-objdump")

ICONV = f"{PYTHON} tools/sjis.py" # TODO: get actual iconv working(?)

#########
# Files #
#########

# Slices
DOL_SLICES = f"{CONFIG}/dol_slices.yml"
REL_SLICES = f"{CONFIG}/rel_slices.yml"

# Overrides (TODO: do these need to be separate for rel?)
ANALYSIS_OVERRIDES = f"{CONFIG}/analysis_overrides.yml"
DISASM_OVERRIDES = f"{CONFIG}/disasm_overrides.yml"

# Binaries
DOL = f"{ORIG}/main.dol" # read in python code
REL = f"{ORIG}/relF.rel" # read in python code
REL_ADDR = "80c45820"
REL_BSS = "80f64340"
DOL_SHA = f"{ORIG}/main.dol.sha1"
REL_SHA = f"{ORIG}/relF.rel.sha1"

# Symbols
SYMBOLS = f"{CONFIG}/symbols.yml"

# Analysis outputs
DOL_LABELS = f"{BUILDDIR}/labels.pickle"
DOL_RELOCS = f"{BUILDDIR}/relocs.pickle"
REL_LABELS = f"{BUILDDIR}/rel_labels.pickle"
REL_RELOCS = f"{BUILDDIR}/rel_relocs.pickle"

# Linker
DOL_LCF = f"{CONFIG}/dol.lcf"
REL_LCF = f"{CONFIG}/rel.lcf"

# Outputs
DOL_ELF = f"{BUILDDIR}/main.elf"
REL_PLF = f"{BUILDDIR}/relF.plf"
DOL_OUT = f"{OUTDIR}/main.dol"
REL_OUT = f"{OUTDIR}/relF.rel"
DOL_MAP = f"{OUTDIR}/main.map"
REL_MAP = f"{OUTDIR}/relF.map"

##############
# Tool Flags #
##############

SDA = "805b5f00 805b7260" #r13 r2

ASFLAGS = ' '.join([
    "-m gekko",
    f"-I {INCDIR}",
    f"-I {PPCDIS_INCDIR}",
    f"-I orig"
])

CFLAGS = [
    "-enc SJIS",
    "-lang c99",
    "-W all",
    "-fp fmadd",
    "-Cpp_exceptions off",
    "-O4",
    "-use_lmw_stmw on",
    "-str pool",
    "-rostr",
    "-sym on",
    "-ipa file"
]
BASE_DOL_CFLAGS = CFLAGS + [
    "-inline all",
    "-sdata 4",
    "-sdata2 4"
]
BASE_REL_CFLAGS = CFLAGS + [
     "-sdata 0",
     "-sdata2 0",
     "-pool off",
     "-ordered-fp-compares"
]

LOCAL_CFLAGS = [
    "-nostdinc",
    "-proc gekko",
    "-maxerrors 1",
    "-I-",
    f"-i {INCDIR}",
    f"-i {PPCDIS_INCDIR}",
    f"-i {BUILD_INCDIR}"
]
DOL_CFLAGS = ' '.join(BASE_DOL_CFLAGS + LOCAL_CFLAGS)
REL_CFLAGS = ' '.join(BASE_REL_CFLAGS + LOCAL_CFLAGS)
EXTERNAL_DOL_CFLAGS = ' '.join(BASE_DOL_CFLAGS)
EXTERNAL_REL_CFLAGS = ' '.join(BASE_REL_CFLAGS)

LDFLAGS = ' '.join([
    "-fp hard",
    "-linkmode moreram",
    "-maxerrors 1",
    "-mapunused"
])

PPCDIS_REL_FLAGS = ' '.join([
    f"-a {REL_ADDR}",
    f"-b {REL_BSS}",
    f"-d {DOL}"
])

PPCDIS_ANALYSIS_FLAGS = ' '.join([
    f"-o {ANALYSIS_OVERRIDES}"
])

PPCDIS_DISASM_FLAGS = ' '.join([
    f"-m {SYMBOLS}",
    f"-o {DISASM_OVERRIDES}"
])

####################
# diff.py Expected #
####################

EXPECTED = "expected"
DOL_EXPECTED = f"{EXPECTED}/build/main.elf"
REL_EXPECTED = f"{EXPECTED}/build/relF.plf"
