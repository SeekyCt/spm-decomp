from argparse import ArgumentParser
from io import StringIO
import os
from sys import executable as PYTHON, platform
from typing import List

from ninja_syntax import Writer


parser = ArgumentParser()
parser.add_argument("tests", type=str, nargs="*", help="Tests to run" \
                    "(mod_ctx, old_mod_ctx, decomp_ctx, mod_ctx_shuffle, test_mod_individual)")
parser.add_argument("--regions", type=str, nargs="+", help="Regions to test")
parser.add_argument("--seed", type=int, default=1, help="Shuffling seed")
parser.add_argument("--shuffle", type=int, default=50, help="Number of randomised orders to test")
parser.add_argument("--individual", action="store_true", help="Test every header on its own")
parser.add_argument("--codewarrior", type=str, help="mwcceppc.exe path")
parser.add_argument("-w", "--wine", type=str, help="Wine override (ignored on Windows)")
args = parser.parse_args()

outbuf = StringIO()
n = Writer(outbuf)

#############
# Variables #
#############

n.variable("builddir", "build")
n.variable("incdir", "include")

n.variable("seed", args.seed)

n.variable("incgen", f"{PYTHON} tools/incgen.py")
n.variable("incgen_single", f"{PYTHON} tools/incgen_single.py")

n.variable("devkitppc", os.environ.get("DEVKITPPC"))
n.variable("cpp", os.path.join("$devkitppc", "bin", "powerpc-eabi-cpp"))

MOD_INCLUDES = ["$incdir", "$mod_incdir"]
n.variable("mod_cc", os.path.join("$devkitppc", "bin", "powerpc-eabi-g++"))
n.variable("mod_incdir", "mod")
n.variable("mod_source", os.path.join("$builddir", "mod.c"))
n.variable(
    "mod_machdep",
    ' '.join([
        "-mno-sdata", # Disable SDA sections since not main binary
        "-DGEKKO", # CPU preprocessor define
        "-mcpu=750", # Set CPU to 750cl
        "-meabi", # Set ppc abi to eabi
        "-mhard-float", # Enable hardware floats
        "-nostdlib", # Don't link std lib
        "-mregnames", # Enable r prefix for registers in asm
        "-ffreestanding", # Tell compiler environment isn't hosted
    ])
)
n.variable(
    "mod_cflags",
    ' '.join([
        "$mod_machdep",
        "$mod_includes",

        "-ffunction-sections", # Allow function deadstripping
        "-fdata-sections", # Allow data deadstripping
        "-g", # Emit debug info
        "-O3", # High optimisation for speed
        "-Wall", # Enable all warnings
        "-Wextra", # Enable even more warnings
        "-Wpedantic", # Enable even more warnings than that
        "-Wshadow", # Enable variable shadowing warning
        "-Werror", # Error on warnings
        "-fmax-errors=1", # Stop after 1 error
    ])
)
n.variable(
    "mod_cxxflags",
    ' '.join([
        "$mod_cflags",

        "-fno-exceptions", # Disable C++ exceptions
        "-fno-rtti", # Disable runtime type info
        "-std=gnu++17", # Use C++17 with GNU extensions
    ])
)

DECOMP_INCLUDES = [
    "$incdir",
    "$decomp_incdir",
]
if args.codewarrior:
    cw = args.codewarrior.replace("/", os.sep)
    if args.wine:
        wine = args.wine
    else:
        wine = "wine"
    if platform != "win32":
        cw = f"{wine} {cw}"
else:
    cw = None
n.variable("decomp_cc", cw)
n.variable("decomp_incdir", "decomp")
n.variable("decomp_source", os.path.join("$builddir", "decomp.c"))
n.variable(
    "decomp_cxxflags",
    ' '.join([
        "-lang c++",
        "-W all",
        "-W err",
        "-fp fmadd",
        "-Cpp_exceptions off",
        "-O4",
        "-use_lmw_stmw on",
        "-str pool",
        "-rostr",
        "-sym dwarf-2",
        "-ipa file",
    ])
)

REGIONS = [
    "eu0",
    "eu1",
    "jp0",
    "jp1",
    "us0",
    "us1",
    "us2",
    "kr0",
]

########
# Rules#
########

ALLOW_CHAIN = "cmd /c " if os.name == "nt" else ""

n.rule(
    "incgen",
    ALLOW_CHAIN + "$incgen $dirs -s $seed -i $iteration > $out"
)

n.rule(
    "incgen_single",
    ALLOW_CHAIN + "$incgen_single $header > $out"
)

n.rule(
    "mod_cc",
    command = "$mod_cc -MMD -MT $out -MF $out.d $mod_cxxflags $flags -c $in -o $out",
    depfile = "$out.d",
    deps = "gcc",
    description = "Mod CC $out",
)

n.rule(
    "decomp_cc",
    command = ALLOW_CHAIN + "$cpp -M $in -MF $out.d $cppflags && " \
                            "$decomp_cc $decomp_cxxflags $flags -c $in -o $out",
    description = "Decomp CC $in",
    deps = "gcc",
    depfile = "$out.d"
)

##########
# Builds #
##########

def incgen(source: str, dirs: List[str], iteration: int = 0):
    n.build(
        source,
        rule="incgen",
        inputs=[],
        variables={
            "dirs" : ' '.join(dirs),
            "iteration" : str(iteration),
        }
    )

def incgen_single(source, header: str):
    n.build(
        source,
        rule="incgen_single",
        inputs=[],
        variables={
            "header" : header,
        }
    )

def compile(dest: str, source: str, includes: List[str], defines: List[str], decomp: bool = False):
    define_flags = ' '.join(f"-D{d}" for d in defines)
    gcc_include_flags = ' '.join(f"-I {d}" for d in includes)
    if decomp:
        mwcc_include_flags = "-I- " + ' '.join(f"-i {d}" for d in includes)
        n.build(
            dest,
            "decomp_cc",
            [source],
            variables={
                "cppflags" : f"{gcc_include_flags} {define_flags}",
                "flags" : f"{mwcc_include_flags} {define_flags}",
            }
        )
    else:
        n.build(
            dest,
            "mod_cc",
            [source],
            variables={
                "flags" : f"{gcc_include_flags} {define_flags}",
            }
        )

def compile_regions(dest: str, source: str, regions: List[str], includes: List[str],
                    defines: List[str], decomp: bool = False):
    for region in regions:
        compile(dest.format(region=region), source, includes, defines + [f"SPM_{region.upper()}"],
                decomp)

def find_headers(dirname: str, base=None) -> List[str]:
    """Returns a list of all headers in a folder recursively"""

    if base is None:
        base = dirname

    ret = []
    for name in os.listdir(dirname):
        path = os.path.join(dirname, name)
        if os.path.isdir(path):
            ret.extend(find_headers(path, base))
        elif name.endswith('.h'):
            ret.append(path[len(base)+1:])

    return ret

# Test the headers in the spm-utils modding setup
def test_mod_ctx(regions: List[str]):
    compile_regions(os.path.join("$builddir", "{region}", "mod.o"), "$mod_source", regions,
                    MOD_INCLUDES, ["USE_STL"])

# Test the headers in the old modding setup
def test_old_mod_ctx(regions: List[str]):
    compile_regions(os.path.join("$builddir", "{region}", "old_mod.o"), "$mod_source", regions,
                    MOD_INCLUDES, [])

# Test the headers in the decomp setup
def test_decomp_ctx(regions: List[str]):
    assert args.codewarrior, "Error: decomp_ctx test requires --codewarrior"
    compile_regions(os.path.join("$builddir", "{region}", "decomp.o"), "$decomp_source", regions,
                    DECOMP_INCLUDES, ["DECOMP", "SKIP_PPCDIS"], True)

# Test shuffled include orders
def test_mod_ctx_shuffle(regions: List[str]):
    assert args.shuffle, "mod_ctx_shuffle test requires --shuffle"
    for i in range(1, 1 + args.shuffle):
        source = os.path.join("$builddir", f"shuffle_{args.seed}", f"{i}.cpp")
        incgen(source, MOD_INCLUDES, i)
        compile_regions(os.path.join("$builddir", "{region}", f"shuffle_mod_{args.seed}_{i}.o"),
                        source, regions, MOD_INCLUDES, ["USE_STL"])

# Test individual headers
def test_mod_individual(regions: List[str]):
    for header in find_headers("include"):
        name = header.removeprefix("include/").removesuffix(".h")
        source = os.path.join("$builddir", "individual", f"{name}.c")
        incgen_single(source, header)
        compile_regions(os.path.join("$builddir", "{region}", "individual", f"{name}.o"), source,
                        regions, MOD_INCLUDES, ["USE_STL", "SPM_EU0"])

test_fns = {
    "mod_ctx" : test_mod_ctx,
    "old_mod_ctx" : test_old_mod_ctx,
    "decomp_ctx" : test_decomp_ctx,
    "mod_ctx_shuffle" : test_mod_ctx_shuffle,
    "test_mod_individual" : test_mod_individual,
}

incgen("$mod_source", MOD_INCLUDES)
default_tests = ["mod_ctx", "old_mod_ctx"]

if args.codewarrior:
    incgen("$decomp_source", DECOMP_INCLUDES)
    default_tests.append("decomp_ctx")

# Add requested tests
tests = set(args.tests if args.tests else default_tests)
regions = args.regions if args.regions else REGIONS
for test in tests:
    if test not in test_fns:
        assert False, f"Error: unknown test {test}"
    else:
        test_fns[test](regions)


with open("build.ninja", 'w') as f:
    f.write(outbuf.getvalue())
n.close()
