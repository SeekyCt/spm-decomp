# Contact

If you're interested in contributing, it's recommended you reach out to us in the #reverse-engineering channel of the [Flipside Tower Discord](https://discord.gg/ndrxwcyCum) first. Here you can ask for any help you might need and discuss what you plan to decompile to prevent any duplicated work.

# spm-headers

This project uses [spm-headers](https://github.com/SeekyCt/spm-headers) for shared game, SDK, NW4R and MSL declarations. It is included in this repository using [git-subrepo](https://github.com/ingydotnet/git-subrepo).

Changes to these declarations should be made in the `spm-headers` directory. The files can be edited like any other tracked files and git-subrepo is not required to build the project, but `spm-headers/.gitrepo` should not be edited manually. See [spm-headers/CONTRIBUTING.md](spm-headers/CONTRIBUTING.md) for its contribution guidelines.

# Source Files

## C++ Linkage

The developers compiled the game as C++ (even with asserts showing that their filenames ended in '.c'), so this project is required to do the same. C++ features are very rarely used, so to avoid the inconvenience of symbol name mangling, all code outside of these rare cases is wrapped in `extern "C"` blocks.

```cpp
#include <common.h>

extern "C" {

// Source definitions

}
```

## Non-Matching Files

Objects marked as `NonMatching` in `configure.py` are not linked in a normal matching build. Add the following warning before the includes in a non-matching DOL source file:

```c
/*
    WARNING: Not fully decompiled
    This file is currently not linked into the final dol
*/
```

For a REL source file, use `rel` instead of `dol`. Remove the warning once the object is marked as matching and is linked in normal builds.

Additionally, list a comment in place of each function that is not yet included as follows:

```c
// NOT_DECOMPILED name
```

# dtk

This project is build around encounter's [decomp-toolkit](https://github.com/encounter/decomp-toolkit).

## Configuration

### config.yml

This file contains base properties for the project. Generally the only time you should need to edit this file is to add assets to the `extract` section.

See also the [dtk-template documentation](https://github.com/encounter/dtk-template/blob/main/config/GAMEID/config.example.yml) of this.

### splits.txt

This file controls the splitting of the game's address space into source files. The address range of a file in each of its sections needs to be added here before it can be decompiled.

Each file added here should also have an entry in configure.py's `config.libs`.

See also the [dtk-template documentation](https://github.com/encounter/dtk-template/blob/main/docs/splits.md) of this.

### symbols.txt

This file controls the naming of functions and data in disassembly.

See also the [dtk-template documentation](https://github.com/encounter/dtk-template/blob/main/docs/symbols.md) of this

## Viewing Original Assembly

dtk will output the assembly of functions to `build/EU0/asm` for manual inspection. For files defined in splits.txt, there will be a file containing all contents of the file. For non-split functions, there will be an individual file for each one.

# objdiff

A setup for objdiff is included as the main way of checking whether functions match, and the progress of files in general.

Download the latest release from [encounter/objdiff](https://github.com/encounter/objdiff). Under project settings, set `Project directory`. The configuration should be loaded automatically.

Select an object from the left sidebar to begin diffing. Changes to the project will rebuild automatically: changes to source files, headers, `configure.py`, `splits.txt` or `symbols.txt`.
