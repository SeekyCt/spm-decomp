# spm-decomp

Decompilation of Super Paper Mario PAL revision 0. This will never be a decompilation of the full game, just specific parts that are useful or interesting, and it certainly won't lead to ports to any other platform. The repo is currently not shiftable (i.e. making changes to the code that causes functions or data to change size or move won't create a working output) but is intended to be at some point. The SDK, NW4R and MSL libraries are out of scope for this project and will not be decompiled (though partial headers will be needed for them to allow other code to be decompiled).

See also https://github.com/SeekyCt/spm-docs for other documentation.

## Building

- Dump a copy of the game and extract the main.dol & relF.bin files
- Extract relF.bin to relF.rel with a tool like [nlzss](https://github.com/magical/nlzss)
- Place main.dol and relF.rel in the orig folder
- Place Codewarrior EPPC Version 4.1 Build 60831 in tools/4199_60831
- Install DevkitPPC, Ninja and Python
    - DevkitPPC r38 and r41 are known to work, other versions probably do too
    - Python 3.8+ is supported, earlier versions may work
- If you didn't use `--recursive` when cloning, run `git submodule update --init --recursive`
- Install the modules from requirements.txt (`pip install -r requirements.txt`)
- Run configure.py
- Run ninja

## Pulling

When pulling the repo later on, make sure to rerun configure.py and `git submodule update --recursive`

## Checking Progress

Running progress.py after a successful build will give the current dol & rel .text section progress.

## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md)

## Modding

In it's current state, the repo isn't really ready for direct editing (the DOL isn't shiftable, the REL might be but it's not been thoroughly tested), but functions can be copied into rel mods and edited there, and the documentation is still useful. 

## Credits
- PistonMiner, Zephiles, Jdaster64, Jasper, NWPlayer123, Malleo, SolidifiedGaming and Diagamma for their TTYD documentation
- All contributors to the [PM64 decomp](https://github.com/ethteck/papermario)
- All contributors to [asm-differ](https://github.com/simonlindholm/asm-differ)
- All contributors to [ppcdis](https://github.com/SeekyCt/ppcdis/)
