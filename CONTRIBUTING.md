# Contact

If you're interested in contributing, it's recommended you reach out to us in the #reverse-engineering channel of the [SPM Speedrunning Discord](https://discord.gg/dbd733H) first. Here you can ask for any help you might need and discuss what you plan to decompile to prevent any duplicated work.

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
