"""
Creates a build script for ninja
"""

from abc import ABC, abstractmethod
from collections import defaultdict
from dataclasses import dataclass
from io import StringIO
import json
import pickle
import os
import re
from typing import List, Tuple

from ninja_syntax import Writer

import common as c

####################
# Setup Validation #
####################

# Check CW was added
assert os.path.exists("tools/4199_60831/mwcceppc.exe") and \
       os.path.exists("tools/4199_60831/mwldeppc.exe"), \
       "Error: Codewarrior not found in tools/4199_60831"

# Check if provided REL is compressed
if not os.path.exists(c.REL) and os.path.exists(c.REL_COMPR):
    # Check the binary is correct
    rel_compr_hash = c.get_file_sha1(c.REL_COMPR)
    if rel_compr_hash == bytes.fromhex("a28a2e23f72f848bd80062e83aba3892951c2e9f"):
        assert 0, "Error: Base rel is from PAL revision 1, the decomp currently requires revision 0"
    else:
        assert rel_compr_hash == bytes.fromhex("3ca98a7ddca976107413c05f3812813af79908f8"), \
            "Error: Base rel hash isn't correct"

    # All good, decompress it
    os.system(f"{c.NLZSS} {c.REL_COMPR} >{c.REL}")

# Check binaries were added
assert os.path.exists(c.DOL) and os.path.exists(c.REL), \
       "Error: Base binaries not found"

# Check binaries are correct
dol_hash = c.get_file_sha1(c.DOL)
assert dol_hash == bytes.fromhex("3fd3d9815528d1c04bc433d67d273b1e3301e976"), \
       "Error: Base dol hash isn't correct."
rel_hash = c.get_file_sha1(c.REL)
if rel_hash == bytes.fromhex("29c78007559996dee6b615005fa3369d4cc1f5e3"):
    assert 0, "Error: Base rel is from PAL revision 1, the decomp currently requires revision 0"
else:
    assert rel_hash == bytes.fromhex("9b9b92c370b1aab68cf6ff5a3eb824fdab6a55ff"), \
        "Error: Base rel hash isn't correct"

# Check submodules added
assert os.path.exists(c.PPCDIS), \
       "Error: Git submodules not initialised"

###############
# Ninja Setup #
###############

outbuf = StringIO()
n = Writer(outbuf)
n.variable("ninja_required_version", "1.3")
n.newline()

################
# Project Dirs #
################

n.variable("builddir", c.BUILDDIR)
n.variable("outdir", c.OUTDIR)
n.variable("orig", c.ORIG)
n.variable("tools", c.TOOLS)
n.variable("config", c.CONFIG)
n.newline()

# This script requires the build folder
os.makedirs(c.BUILDDIR, exist_ok=True)

#########
# Tools #
#########

n.variable("python", c.PYTHON)
n.variable("ppcdis", c.PPCDIS)
n.variable("relextern", c.RELEXTERN)
n.variable("analyser", c.ANALYSER)
n.variable("disassembler", c.DISASSEMBLER)
n.variable("orderstrings", c.ORDERSTRINGS)
n.variable("orderfloats", c.ORDERFLOATS)
n.variable("assetrip", c.ASSETRIP)
n.variable("assetinc", c.ASSETINC)
n.variable("forceactivegen", c.FORCEACTIVEGEN)
n.variable("elf2dol", c.ELF2DOL)
n.variable("elf2rel", c.ELF2REL)
n.variable("codewarrior", c.CODEWARRIOR)
n.variable("cc", c.CC)
n.variable("ld", c.LD)
n.variable("devkitppc", c.DEVKITPPC)
n.variable("as", c.AS)
n.variable("cpp", c.CPP)
n.variable("iconv", c.ICONV)
n.newline()

##############
# Tool flags #
##############

n.variable("asflags", c.ASFLAGS)
n.variable("cppflags", c.CPPFLAGS)
n.variable("ldflags", c.LDFLAGS)
n.variable("ppcdis_analysis_flags", c.PPCDIS_ANALYSIS_FLAGS)
n.variable("ppcdis_disasm_flags", c.PPCDIS_DISASM_FLAGS)
n.newline()

#########
# Rules #
#########

# Windows can't use && without this
ALLOW_CHAIN = "cmd /c " if os.name == "nt" else ""

n.rule(
    "relextern",
    command = "$relextern $out $in",
    description = "ppcdis rel extern $in"
)

n.rule(
    "analyse",
    command = "$analyser $in $out $analysisflags",
    description = "ppcdis analysis $in",
    pool="console"
)

n.rule(
    "disasm",
    command = "$disassembler $in $out -q $disasmflags",
    description = "ppcdis full disassembly $out"
)

n.rule(
    "disasm_slice",
    command = "$disassembler $in $out -q $disasmflags -s $slice",
    description = "ppcdis disassembly $out"
)

n.rule(
    "disasm_single",
    command = "$disassembler $in $out -f $addr -i -q $disasmflags",
    description = "ppcdis function disassembly $addr"
)

n.rule(
    "jumptable",
    command = "$disassembler $in $out -j $addr -q $disasmflags",
    description = "Jumptable $addr"
)

n.rule(
    "orderstrings",
    command = "$orderstrings $in $addrs $out $flags --enc shift-jis --pool",
    description = "Order strings $in $addrs"
)

n.rule(
    "orderfloats",
    command = "$orderfloats $in $addrs $out $flags",
    description = "Order floats $in $addrs"
)

n.rule(
    "assetrip",
    command = "$assetrip $in $addrs $out",
    description = "Asset rip $out"
)

n.rule(
    "assetinc",
    command = "$assetinc $in $out",
    description = "Asset include generation $out"
)

n.rule(
    "forceactivegen",
    command = "$forceactivegen $in $out",
    description = "LCF FORCEACTIVE generation $in"
)

n.rule(
    "elf2dol",
    command = "$elf2dol $in -o $out",
    description = "elf2dol $in"
)

n.rule(
    "elf2rel",
    command = "$elf2rel $in -o $out $flags",
    description = "elf2rel $in"
)

n.rule(
    "sha1sum",
    command = ALLOW_CHAIN + "sha1sum -c $in && touch $out",
    description = "Verify $in",
    pool="console"
)

n.rule(
    "as",
    command = f"$as $asflags -c $in -o $out",
    description = "AS $in"
)

n.rule(
    "cc",
    command = ALLOW_CHAIN + f"$cpp -M $in -MF $out.d $cppflags && $cc $cflags -c $in -o $out",
    description = "CC $in",
    deps = "gcc",
    depfile = "$out.d"
)

n.rule(
    "ccs",
    command = ALLOW_CHAIN + f"$cpp -M $in -MF $out.d $cppflags && $cc $cflags -S $in -o $out",
    description = "CC -S $in",
    deps = "gcc",
    depfile = "$out.d"
)

n.rule(
    "ld",
    command = "$ld $ldflags -map $map -lcf $lcf $in -o $out",
    description = "LD $out",
)

n.rule(
    "iconv",
    command = "$iconv $in $out",
    description = "iconv $in",
)

##########
# Assets #
##########

@dataclass
class Asset:
    binary: str
    path: str
    start: int
    end: int

    def load(yml_path: str):
        return {
            asset : Asset(binary, asset, *adat["addrs"])
            for binary, bdat in c.load_from_yaml(yml_path).items()
            for asset, adat in bdat.items()
        }

assets = Asset.load(c.ASSETS_YML)

###########
# Sources #
###########

class GeneratedInclude(ABC):
    def __init__(self, ctx: c.SourceContext, source_name: str, path: str):
        self.ctx = ctx
        self.source_name = source_name
        self.path = path

    @abstractmethod
    def build(self):
        raise NotImplementedError

    def find(ctx: c.SourceContext, source_name: str, txt: str) -> List["GeneratedInclude"]:
        return [
            cl(ctx, source_name, match)
            for cl in (
                AsmInclude,
                JumptableInclude,
                StringInclude,
                FloatInclude,
                DoubleInclude,
                AssetInclude
            )
            for match in re.findall(cl.REGEX, txt)
        ]

class AsmInclude(GeneratedInclude):
    REGEX = r'#include "asm\/([0-9a-f]{8})\.s"'

    def __init__(self, ctx: c.SourceContext, source_name: str, match: str):
        self.addr = match
        super().__init__(ctx, source_name, f"{c.BUILD_INCDIR}/asm/{self.addr}.s")

    def build(includes: List["AsmInclude"]):
        # Skip empty list
        if len(includes) == 0:
            return

        # Get ctx from first include (all should be equal)
        ctx = includes[0].ctx

        # Sort by source name
        batches = defaultdict(list)
        for inc in includes:
            batches[inc.source_name].append(inc)
        
        # Compile by source name
        # TODO: subdivide large batches
        for source_name, incs in batches.items():
            n.build(
                [inc.path for inc in incs],
                rule="disasm_single",
                inputs=[ctx.binary, ctx.labels, ctx.relocs],
                implicit=[c.GAME_SYMBOLS, c.DISASM_OVERRIDES],
                variables={
                    "disasmflags" : f"$ppcdis_disasm_flags -n {source_name}",
                    "addr" : ' '.join(inc.addr for inc in incs)
                }
            )
    
    def __repr__(self):
        return f"AsmInclude({self.addr})"

class JumptableInclude(GeneratedInclude):
    REGEX = r'#include "jumptable\/([0-9a-f]{8})\.inc"'

    def __init__(self, ctx: c.SourceContext, source_name: str, match: str):
        self.addr = match
        super().__init__(ctx, source_name, f"{c.BUILD_INCDIR}/jumptable/{self.addr}.inc")
    
    def build(includes: List["JumptableInclude"]):
        # Skip empty list
        if len(includes) == 0:
            return

        # Get context from first include (all should be equal)
        ctx = includes[0].ctx

        # Sort by source name
        batches = defaultdict(list)
        for inc in includes:
            batches[inc.source_name].append(inc)
        
        # Compile by source name
        # TODO: subdivide large batches
        for source_name, incs in batches.items():
            n.build(
                [inc.path for inc in incs],
                rule="jumptable",
                inputs=[ctx.binary, ctx.labels, ctx.relocs],
                implicit=[c.GAME_SYMBOLS, c.DISASM_OVERRIDES],
                variables={
                    "disasmflags" : f"$ppcdis_disasm_flags -n {source_name}",
                    "addr" : ' '.join(inc.addr for inc in incs)
                }
            )

    def __repr__(self):
        return f"JumptableInclude({self.addr})"

class StringInclude(GeneratedInclude):
    REGEX = r'#include "orderstrings\/([0-9a-f]{8})_([0-9a-f]{8})\.inc"'

    def __init__(self, ctx: c.SourceContext, source_name: str, match: Tuple[str]):
        self.start, self.end = match
        super().__init__(ctx, source_name,
                         f"{c.BUILD_INCDIR}/orderstrings/{self.start}_{self.end}.inc")

    def build(includes: List["StringInclude"]):
        # Skip empty list
        if len(includes) == 0:
            return

        # Get context from first include (all should be equal)
        ctx = includes[0].ctx

        # Build
        for inc in includes:
            n.build(
                inc.path,
                rule="orderstrings",
                inputs=ctx.binary,
                variables={
                    "addrs" : f"{inc.start} {inc.end}"
                }
            )

    def __repr__(self):
        return f"StringInclude({self.start}, {self.end})"

class FloatInclude(GeneratedInclude):
    REGEX = r'#include "(orderfloats(m?))\/([0-9a-f]{8})_([0-9a-f]{8})\.inc"'

    def __init__(self, ctx: c.SourceContext, source_name: str, match: Tuple[str]):
        folder, manual, self.start, self.end = match
        self.manual = manual != ''
        super().__init__(ctx, source_name,
                         f"{c.BUILD_INCDIR}/{folder}/{self.start}_{self.end}.inc")

    def build(includes: List["FloatInclude"]):
        # Skip empty list
        if len(includes) == 0:
            return

        # Get context from first include (all should be equal)
        ctx = includes[0].ctx

        # Build
        for inc in includes:
            sda = "--sda " if ctx.sdata2_threshold >= 4 else ""
            asm = "" if inc.manual else "--asm"
            n.build(
                inc.path,
                rule="orderfloats",
                inputs=inc.ctx.binary,
                variables={
                    "addrs" : f"{inc.start} {inc.end}",
                    "flags" : f"{sda} {asm}"
                }
            )

    def __repr__(self):
        return f"FloatInclude({self.start}, {self.end})"

class DoubleInclude(GeneratedInclude):
    REGEX = r'#include "orderdoubles\/([0-9a-f]{8})_([0-9a-f]{8})\.inc"'

    def __init__(self, ctx: c.SourceContext, source_name: str, match: Tuple[str]):
        self.start, self.end = match
        super().__init__(ctx, source_name,
                         f"{c.BUILD_INCDIR}/orderdoubles/{self.start}_{self.end}.inc")

    def build(includes: List["DoubleInclude"]):
        # Skip empty list
        if len(includes) == 0:
            return

        # Get context from first include (all should be equal)
        ctx = includes[0].ctx

        # Build
        for inc in includes:
            n.build(
                inc.path,
                rule="orderfloats",
                inputs=ctx.binary,
                variables={
                    "addrs" : f"{inc.start} {inc.end}",
                    "flags" : f"--double"
                }
            )

    def __repr__(self):
        return f"DoubleInclude({self.start}, {self.end})"

class AssetInclude(GeneratedInclude):
    REGEX = r'#include "assets\/(.+)\.inc"'

    def __init__(self, ctx: c.SourceContext, source_name: str, match: Tuple[str]):
        self.asset = assets[match]
        self.asset_path = f"{c.ASSETS}/{self.asset.path}"
        assert ctx.binary == self.asset.binary, f"Tried to include from other binary"
        super().__init__(ctx, source_name, f"{c.BUILD_INCDIR}/assets/{self.asset.path}.inc")

    def build(includes: List["AssetInclude"]):
        # Skip empty list
        if len(includes) == 0:
            return

        # Build
        for inc in includes:
            n.build(
                inc.asset_path,
                rule="assetrip",
                inputs=inc.asset.binary,
                variables={
                    "addrs" : f"{inc.asset.start:x} {inc.asset.end:x}"
                }
            )
            n.build(
                inc.path,
                rule="assetinc",
                inputs=inc.asset_path
            )

    def __repr__(self):
        return f"AssetInclude({self.asset})"

class Source(ABC):
    def __init__(self, decompiled: bool, src_path: str, o_path: str,
                 gen_includes: List[GeneratedInclude] = []):
        self.decompiled = decompiled
        self.src_path = src_path
        self.o_path = o_path
        self.o_stem = o_path[:-2]
        self.gen_includes = gen_includes

    def build(self):
        raise NotImplementedError
    
    def make(ctx: c.SourceContext, source: c.SourceDesc):
        if isinstance(source, str):
            ext = source.split('.')[-1].lower()
            if ext in ("c", "cpp", "cxx", "cc"):
                return CSource(ctx, source)
            elif ext == "s":
                return AsmSource(ctx, source)
            else:
                assert 0, f"Unknown source type .{ext}"
        else:
            return GenAsmSource(ctx, *source)

class GenAsmSource(Source):
    def __init__(self, ctx: c.SourceContext, section: str, start: int, end: int):
        self.start = start
        self.end = end
        self.ctx = ctx
        src_path = f"$builddir/asm/{section}_{start:x}_{end:x}.s"
        super().__init__(False, src_path, src_path + ".o")

    def build(self):
        n.build(
            self.src_path,
            rule = "disasm_slice",
            inputs = [self.ctx.binary, self.ctx.labels, self.ctx.relocs],
            implicit = [c.GAME_SYMBOLS, c.DISASM_OVERRIDES],
            variables = {
                "slice" : f"{self.start:x} {self.end:x}",
                "disasmflags" : f"$ppcdis_disasm_flags"
            }
        )
        n.build(
            self.o_path,
            rule="as",
            inputs=self.src_path
        )
    
    def batch_build(sources: List["GenAsmSource"], batch_size=20):
        # TODO: configure batch size based on cpu core count

        # Skip empty list
        if len(sources) == 0:
            return

        # Get context from first include (all should be equal)
        ctx = sources[0].ctx

        for src in sources:
            n.build(
                src.o_path,
                rule="as",
                inputs=src.src_path
            )

        while len(sources) > 0:
            batch, sources = sources[:batch_size], sources[batch_size:]
            n.build(
                [src.src_path for src in batch],
                rule = "disasm_slice",
                inputs = [ctx.binary, ctx.labels, ctx.relocs],
                implicit = [c.GAME_SYMBOLS, c.DISASM_OVERRIDES],
                variables = {
                    "slice" : ' '.join(
                        f"{src.start:x} {src.end:x}"
                        for src in batch
                    ),
                    "disasmflags" : f"$ppcdis_disasm_flags"
                }
            )

class AsmSource(Source):
    def __init__(self, ctx: c.SourceContext, path: str):
        super().__init__(True, path, f"$builddir/{path}.o")

    def build(self):
        n.build(
            self.o_path,
            rule = "as",
            inputs = self.src_path
        )


class CSource(Source):
    def __init__(self, ctx: c.SourceContext, path: str):
        self.cflags = ctx.cflags
        self.iconv_path = f"$builddir/iconv/{path}"

        # Find generated includes
        with open(path, encoding="utf-8") as f:
            gen_includes = GeneratedInclude.find(ctx, path, f.read())

        self.s_path = f"$builddir/{path}.s"
        super().__init__(True, path, f"$builddir/{path}.o", gen_includes)

    def build(self):
        n.build(
            self.iconv_path,
            rule="iconv",
            inputs=self.src_path
        )
        n.build(
            self.o_path,
            rule = "cc",
            inputs = self.iconv_path,
            implicit = [inc.path for inc in self.gen_includes],
            variables = {
                "cflags" : self.cflags
            }
        )
        # Optional manual debug target
        n.build(
            self.s_path,
            rule = "ccs",
            inputs = self.iconv_path,
            implicit = [inc.path for inc in self.gen_includes],
            variables = {
                "cflags" : self.cflags
            }
        )

def load_sources(ctx: c.SourceContext):
    raw = c.get_cmd_stdout(
        f"{c.SLICES} {ctx.binary} {ctx.slices} -o -p {ctx.srcdir}/"
    )
    return [Source.make(ctx, s) for s in json.loads(raw)]

def find_gen_includes(sources: List[Source]):
    ret = defaultdict(list)
    for source in sources:
        if not isinstance(source, CSource):
            continue
        
        for inc in source.gen_includes:
            ret[type(inc)].append(inc)

    return ret

def make_asm_list(path: str, asm_includes: List[AsmInclude]):
    with open(path, 'wb') as f:
        pickle.dump(
            [
                int(inc.addr, 16)
                for inc in asm_includes
            ],
            f
        )

dol_sources = load_sources(c.DOL_CTX)
dol_gen_includes = find_gen_includes(dol_sources)
make_asm_list(c.DOL_ASM_LIST, dol_gen_includes[AsmInclude])

rel_sources = load_sources(c.REL_CTX)
rel_gen_includes = find_gen_includes(rel_sources)
make_asm_list(c.REL_ASM_LIST, rel_gen_includes[AsmInclude])

##########
# Builds #
##########

n.build(
    c.EXTERNS,
    rule = "relextern",
    inputs = c.REL_YML
)

n.build(
    [c.REL_LABELS, c.REL_RELOCS],
    rule = "analyse",
    inputs = c.REL_YML,
    implicit = [c.ANALYSIS_OVERRIDES, c.EXTERNS],
    variables = {
        "analysisflags" : "$ppcdis_analysis_flags"
    }
)

n.build(
    [c.DOL_LABELS, c.DOL_RELOCS],
    rule = "analyse",
    inputs = c.DOL_YML,
    implicit = [c.ANALYSIS_OVERRIDES, c.EXTERNS],
    variables = {
        "analysisflags" : f"$ppcdis_analysis_flags"
    }
)

for cl, includes in dol_gen_includes.items():
    cl.build(includes)

for cl, includes in rel_gen_includes.items():
    cl.build(includes)

dol_gen_asm = []
for source in dol_sources:
    if isinstance(source, GenAsmSource):
        dol_gen_asm.append(source)
    else:
        source.build()
GenAsmSource.batch_build(dol_gen_asm)

rel_gen_asm = []
for source in rel_sources:
    if isinstance(source, GenAsmSource):
        rel_gen_asm.append(source)
    else:
        source.build()
GenAsmSource.batch_build(rel_gen_asm)

n.build(
    c.DOL_LCF,
    rule="forceactivegen",
    inputs=[c.DOL_LCF_TEMPLATE, c.DOL_YML, c.DOL_LABELS, c.GAME_SYMBOLS, c.EXTERNS]
)

n.build(
    c.DOL_ELF,
    rule="ld",
    inputs=[s.o_path for s in dol_sources],
    implicit=c.DOL_LCF,
    implicit_outputs=c.DOL_MAP,
    variables={
        "map" : c.DOL_MAP,
        "lcf" : c.DOL_LCF
    }
)

n.build(
    c.DOL_OUT,
    rule="elf2dol",
    inputs=c.DOL_ELF,
)

n.build(
    c.DOL_OK,
    rule = "sha1sum",
    inputs = c.DOL_SHA,
    implicit = [c.DOL_OUT]
)
n.default(c.DOL_OK)

n.build(
    c.REL_PLF,
    rule="ld",
    inputs=[s.o_path for s in rel_sources],
    implicit=c.REL_LCF,
    implicit_outputs=c.REL_MAP,
    variables={
        "map" : c.REL_MAP,
        "lcf" : c.REL_LCF,
        "ldflags" : c.LDFLAGS + " -r1"
    }
)

n.build(
    c.REL_OUT,
    rule="elf2rel",
    inputs=[c.REL_PLF, c.DOL_ELF],
    variables={
        "flags" : f"-n 18 --name-size 0x40 -r {c.REL_YML}"
    }
)

n.build(
    c.REL_OK,
    rule = "sha1sum",
    inputs = c.REL_SHA,
    implicit = [c.REL_OUT]
)
n.default(c.REL_OK)

# Optional full binary disassembly
n.build(
    c.DOL_FULL,
    rule = "disasm",
    inputs=[c.DOL_YML, c.DOL_LABELS, c.DOL_RELOCS],
    implicit=[c.GAME_SYMBOLS, c.DISASM_OVERRIDES],
    variables={
        "disasmflags" : "$ppcdis_disasm_flags"
    }
)

n.build(
    c.REL_FULL,
    rule = "disasm",
    inputs=[c.REL_YML, c.REL_LABELS, c.REL_RELOCS],
    implicit=[c.GAME_SYMBOLS, c.DISASM_OVERRIDES],
    variables={
        "disasmflags" : "$ppcdis_disasm_flags"
    }
)

##########
# Output #
##########

with open("build.ninja", 'w') as f:
    f.write(outbuf.getvalue())
n.close()
