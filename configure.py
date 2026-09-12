#!/usr/bin/env python3

# Based on dtk-template b0eff0abd44be06aa857e62368fc9986bcb3a86d

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "EU0",  # 0
    "EU1",  # 1
    "JP0",  # 2
    "KR0",  # 3
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--ninja",
    metavar="BINARY",
    type=Path,
    help="path to ninja binary (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.ninja_path = args.ninja
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20251118"
config.dtk_tag = "v1.8.0"
config.objdiff_tag = "v3.5.1"
config.sjiswrap_tag = "v1.2.2"
config.wibo_tag = "1.0.0"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
    f"--defsym VERSION_{config.version}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")
if args.map:
    config.ldflags.append("-mapunused")

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
DECOMPME_PRESET_DOL = 74
DECOMPME_PRESET_REL = 75
config.scratch_preset_id = DECOMPME_PRESET_DOL

SPM_HEADERS = "spm-headers"

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions on",
    "-W all,error",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-enc SJIS",
    "-i include",
    f"-i build/{config.version}/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",

    # spm-headers
    "-DDECOMP",
    f"-DSPM_{config.version.upper()}",
    f"-i {SPM_HEADERS}/include",
    f"-i {SPM_HEADERS}/decomp",
]

# Debug flags
if args.debug:
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
]

# SPM dol flags
cflags_dol = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-lang c++",
    "-fp fmadd", # TODO: needed?
    "-str pool,reuse,readonly",
    "-ipa file",
    "-inline all",
    "-sdata 4",
    "-sdata2 4",
    "-sym on", # Enable line numbers in objdiff
]

# REL flags
cflags_rel = [
    *cflags_dol,
    "-sdata 0",
    "-sdata2 0",
]

config.linker_version = "GC/3.0a5.2"


# Helper functions for libraries

def Runtime(*objects: Object) -> Dict[str, Any]:
    return {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "objects": objects,
    }

def Dol(*objects: Object) -> Dict[str, Any]:
    return {
        "lib": "dol",
        "src_dir": "src",
        "mw_version": config.linker_version,
        "cflags": cflags_dol,
        "objects": objects,
    }


def Rel(*objects: Object) -> Dict[str, Any]:
    return {
        "lib": "rel",
        "src_dir": "rel",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "objects": objects,
        "scratch_preset_id": DECOMPME_PRESET_REL,
    }


Matching = True                   # Object matches and should be linked
NonStarted = False                # Object has no source file created
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    Dol(
        Object(Matching, "main.c"),
        Object(NonStarted, "ac_shake_timing.c"),
        Object(NonStarted, "ac_pendulum_crane_timing.c"),
        Object(NonStarted, "ac_scissor.c"),
        Object(NonStarted, "ac_crane_timing.c"),
        Object(NonStarted, "ac_signal_timing.c"),
        Object(NonStarted, "ac_target_row.c"),
        Object(NonStarted, "ac_timing_button.c"),
        Object(NonStarted, "ac_timing_hold.c"),
        Object(NonStarted, "item_event_data.c"),
        Object(NonStarted, "map_data.c"),
        Object(NonStarted, "setup_data.c"),
        Object(NonStarted, "mobjdrv.c"),
        Object(NonStarted, "msgdrv.c"),
        Object(NonStarted, "offscreen.c"),
        Object(NonStarted, "shadowdrv.c"),
        Object(Matching, "swdrv.c"),
        Object(NonMatching, "windowdrv.c"),
        Object(NonStarted, "winmgr.c"),
        Object(NonStarted, "acdrv.c"),
        Object(NonStarted, "animdrv.c"),
        Object(NonStarted, "bgdrv.c"),
        Object(NonStarted, "cam_road.c"),
        Object(NonStarted, "cam_shift.c"),
        Object(NonStarted, "camdrv.c"),
        Object(NonStarted, "casedrv.c"),
        Object(NonStarted, "dispdrv.c"),
        Object(NonStarted, "eff_sub.c"),
        Object(NonStarted, "effdrv.c"),
        Object(NonStarted, "envdrv.c"),
        Object(NonStarted, "extdrv.c"),
        Object(NonStarted, "fadedrv.c"),
        Object(NonStarted, "framedrv.c"),
        Object(NonStarted, "hitdrv.c"),
        Object(NonStarted, "icondrv.c"),
        Object(NonStarted, "imgdrv.c"),
        Object(NonStarted, "itemdrv.c"),
        Object(NonStarted, "lightdrv.c"),
        Object(NonStarted, "mapdrv.c"),
        Object(NonStarted, "eff_spm_recovery.c"),
        Object(NonStarted, "eff_zunbaba.c"),
        Object(NonStarted, "eff_spm_use.c"),
        Object(NonStarted, "eff_spm_get.c"),
        Object(NonStarted, "eff_map_water.c"),
        Object(NonStarted, "eff_pure_heart.c"),
        Object(NonStarted, "eff_map_bubble.c"),
        Object(NonStarted, "eff_score.c"),
        Object(NonStarted, "evt_snd.c"),
        Object(NonStarted, "evt_sub.c"),
        Object(Matching, "evtmgr.c"),
        Object(Matching, "evtmgr_cmd.c"),
        Object(NonStarted, "evt_ac.c"),
        Object(NonStarted, "evt_bg.c"),
        Object(NonStarted, "evt_cam.c"),
        Object(NonStarted, "evt_case.c"),
        Object(NonStarted, "evt_door.c"),
        Object(NonStarted, "evt_eff.c"),
        Object(NonStarted, "evt_env.c"),
        Object(NonStarted, "evt_ext.c"),
        Object(NonStarted, "evt_fade.c"),
        Object(NonStarted, "evt_fairy.c"),
        Object(NonStarted, "evt_frame.c"),
        Object(NonStarted, "evt_guide.c"),
        Object(NonStarted, "evt_hit.c"),
        Object(NonStarted, "evt_img.c"),
        Object(NonStarted, "evt_item.c"),
        Object(NonStarted, "evt_map.c"),
        Object(NonStarted, "evt_mario.c"),
        Object(NonStarted, "evt_mobj.c"),
        Object(NonStarted, "evt_msg.c"),
        Object(NonStarted, "evt_npc.c"),
        Object(NonStarted, "evt_offscreen.c"),
        Object(NonStarted, "evt_paper.c"),
        Object(NonStarted, "evt_pouch.c"),
        Object(NonStarted, "evt_seq.c"),
        Object(NonStarted, "evt_shop.c"),
        Object(NonStarted, "mot_stay.c"),
        Object(NonStarted, "mot_swim.c"),
        Object(NonStarted, "mot_mega_star.c"),
        Object(NonStarted, "mot_walk.c"),
        Object(NonStarted, "mot_crouch.c"),
        Object(NonStarted, "mot_spring.c"),
        Object(NonStarted, "mot_ladder.c"),
        Object(NonStarted, "fairy.c"),
        Object(NonStarted, "guide.c"),
        Object(NonStarted, "mario.c"),
        Object(NonStarted, "mario_acrobat.c"),
        Object(NonStarted, "mario_fairy.c"),
        Object(NonStarted, "mario_hit.c"),
        Object(NonStarted, "mario_motion.c"),
        Object(NonMatching, "mario_pouch.c"),
        Object(NonStarted, "mario_sbr.c"),
        Object(NonStarted, "mario_status.c"),
        Object(NonStarted, "mot_fairy_other.c"),
        Object(NonStarted, "mot_jabara.c"),
        Object(NonStarted, "mot_jump.c"),
        Object(NonStarted, "mot_hit.c"),
        Object(NonStarted, "mot_peach_float.c"),
        Object(NonStarted, "mot_slit.c"),
        Object(NonStarted, "seq_gameover.c"),
        Object(NonStarted, "seq_logo.c"),
        Object(NonStarted, "seq_mapchange.c"),
        Object(Matching, "seq_title.c"),
        Object(Matching, "seqdef.c"),
        Object(Matching, "seqdrv.c"),
        Object(NonStarted, "seq_load.c"),
        Object(NonStarted, "seq_game.c"),
        Object(NonStarted, "seq_load_sub.c"),
        Object(NonStarted, "levelupwindow.c"),
        Object(NonStarted, "charmwindow.c"),
        Object(NonStarted, "nameent.c"),
        Object(NonStarted, "pausewin.c"),
        Object(NonStarted, "charpixlitemwin.c"),
        Object(NonStarted, "hud.c"),
        Object(NonStarted, "treasuremap.c"),
        Object(NonStarted, "sptexture.c"),
        Object(NonMatching, "system.c"),
        Object(Matching, "dvdmgr.c"),
        Object(Matching, "debug.c"),
        Object(Matching, "filemgr.c"),
        Object(NonStarted, "fontmgr.c"),
        Object(NonStarted, "gxsub.c"),
        Object(NonStarted, "himo.c"),
        Object(Matching, "memory.c"),
        Object(NonStarted, "parse.c"),
        Object(NonStarted, "romfont.c"),
        Object(NonMatching, "spmario.c"),
        Object(NonStarted, "DEMOInit.c"),
        Object(NonStarted, "npcdrv.c"),
        Object(NonMatching, "wpadmgr.c"),
        Object(NonStarted, "spmario_snd.c"),
        Object(Matching, "relmgr.c"),
        Object(Matching, "nandmgr.c"),
        Object(NonStarted, "ac_shake.c"),
        Object(NonStarted, "evt_nand.c"),
        Object(NonStarted, "homebuttondrv.c"),
        Object(NonStarted, "search.c"),
        Object(NonStarted, "evt_search.c"),
        Object(NonStarted, "eff_cheri_shell.c"),
    ),
    Rel(
        Object(NonMatching, "dan.c"),
    ),
    Runtime(
        Object(NonMatching, "Runtime.PPCEABI.H/global_destructor_chain.c"),
        Object(NonMatching, "Runtime.PPCEABI.H/__init_cpp_exceptions.cpp"),
    ),
]


# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["dummy.c"]
    return objects

# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback


# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    # Unused since full splits not known
]
config.progress_each_module = args.verbose
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    # Marks relocations as mismatching if the target value is different
    # Default is "functionRelocDiffs=none", which is most lenient
    # "--config functionRelocDiffs=data_value",
]

# Custom clangd flags
config.extra_clang_flags = [
    "-fshort-wchar", # 16-bit wchar_t to match CW
    "-Wno-main", # Don't warn for extern C main
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    print("Configure for", args.version)
    generate_build(config)
elif args.mode == "progress":
    # Print progress information
    config.progress_each_module = args.verbose
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
