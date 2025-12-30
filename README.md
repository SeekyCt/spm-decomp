# spm-decomp
[![Code Progress]][progress] [![Data Progress]][progress] [![Discord Badge]][discord]
=============

[Code Progress]: https://decomp.dev/SeekyCt/spm-decomp.svg?mode=shield&measure=code&label=Code
[Data Progress]: https://decomp.dev/SeekyCt/spm-decomp.svg?mode=shield&measure=data&label=Data
[progress]: https://decomp.dev/SeekyCt/spm-decomp
[Discord Badge]: https://img.shields.io/discord/727908905392275526?color=%237289DA&logo=discord&logoColor=%23FFFFFF
[discord]: https://discord.gg/dbd733H

Decompilation of Super Paper Mario. The main versions targetted are the two PAL versions of the game (with symbol and split addresses being identical between both). Support has also been set-up for NTSC-U revision 0 of the game, though less addresses have been ported (and it's advised to not put work into this until some kind of automated porting setup has been created).

This will never be a decompilation of the full game, just specific parts that are useful or interesting, and it certainly won't lead to ports to any other platform. In theory the repo is shiftable (i.e. files marked as `Matching` in configure.py can be edited to change the behaviour of the game) but this hasn't been thorougly tested. The SDK, NW4R and MSL libraries are out of scope for this project and will not be decompiled (though partial headers will be needed for them to allow other code to be decompiled).

See also https://github.com/SeekyCt/spm-docs for other documentation.

## Dependency Setup

### Windows

On Windows, it's **highly recommended** to use native tooling. WSL or msys2 are **not** required.  
When running under WSL, objdiff is unable to get filesystem notifications for automatic rebuilds.

- Install [Python](https://www.python.org/downloads/) and add it to `%PATH%`.
  - Also available from the [Windows Store](https://apps.microsoft.com/store/detail/python-311/9NRWMJP3717K).
- Download [ninja](https://github.com/ninja-build/ninja/releases) and add it to `%PATH%`.
  - Quick install via pip: `pip install ninja`

### macOS

- Install [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages):

  ```sh
  brew install ninja
  ```

[wibo](https://github.com/decompals/wibo), a minimal 32-bit Windows binary wrapper, will be automatically downloaded and used.

### Linux

- Install [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages).

[wibo](https://github.com/decompals/wibo), a minimal 32-bit Windows binary wrapper, will be automatically downloaded and used.

## Building

- Copy your game's disc image to the subfolder of `orig` for your version
  - Supported versions:
    - EU0: PAL revision 0
    - EU1: PAL revision 1
    - JP0: NTSC-J revision 0
    - KR0: NTSC-K revision 0
  - Supported formats: ISO (GCM), RVZ, WIA, WBFS, CISO, NFS, GCZ, TGC
  - After the initial build, the disc image can be deleted to save space.
- Run configure.py
  - For EU0, no additional arguments are required
  - For any other version, use `--version RGX` where `RGX` is the region and revision of your version
- Run ninja

## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md)

## Modding

In its current state, the repo isn't really ready for direct editing, but functions can be copied into [REL mods](https://github.com/SeekyCt/spm-rel-loader) and edited there, and the documentation is still useful. 

## Credits
- PistonMiner, Zephiles, Jdaster64, Jasper, NWPlayer123, Malleo, SolidifiedGaming and Diagamma for their TTYD documentation
- All contributors to the [PM64 decomp](https://github.com/ethteck/papermario)
- All contributors to [asm-differ](https://github.com/simonlindholm/asm-differ)
- All contributors to [decomp-toolkit](https://github.com/encounter/decomp-toolkit) and [dtk-template](https://github.com/encounter/dtk-template/)
