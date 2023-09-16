# spm-headers
WIP header library for Super Paper Mario.

# Setup

## General

One of the following preprocessor defines should be used for the target game version:
- `SPM_JP0`
- `SPM_JP1`
- `SPM_US0`
- `SPM_US1`
- `SPM_US2`
- `SPM_EU0`
- `SPM_EU1`
- `SPM_KR0`

It is recommended to use this with [git subrepo](https://github.com/ingydotnet/git-subrepo).

## Decomp

For use in decomp, the `include` and `decomp` folders should be added to the include path, and the preprocessor define `DECOMP` should be used.

### Testing

The `SKIP_PPCDIS` preprocessor define will stop `ppcdis.h` being included.

## Mods

For use in mods, the `include` and `mod` folder should be added to the include path and an lst from `linker` should be used. If including your compiler's C++ standard library, the preprocessor define `USE_STL` should be used.

# Licensing

All code originally written for this project (everything under the `include`, `decomp` and `linker` directories) is available under the MIT license.

Everything under the `mod` folder is available under the GPLv3 license as it's derived from other GPL code.

# Symbols

Currently, symbols are manually added and ported as needed. Any symbols that are missing from the lsts here can be found in the [decomp symbols yml](https://github.com/SeekyCt/spm-decomp/blob/master/config/symbols.yml) for eu0. In the future, that yml will become part of this repo and the lsts will be auto generated based on it.

# Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on contributing.
