# spm-decomp

Decompilation of Super Paper Mario (PAL revision 0). Doesn't produce a dol, asm is just checked by compiling with -S. A small bit of TTYD support.

See also https://github.com/SeekyCt/spm-docs for other documentation

## Address Notes
- Addresses of global functions and variables are commented in headers, static ones are commented at their c file definitions
- PAL revision 1's addresses are identical to revision 0's, so information from the decomp is accurate there too
- Rel addresses are given for the rel loaded in its default position for PAL revisions 0 & 1 (80c45820)

## Compiler Notes
### Flags
The compiler flags probably aren't fully accurate yet, and it's likely that some parts of the game will use different flags.
### Version
The project currently uses Codewarrior Version 4.1 build 60831, it's not known if this is the correct version.

## Matching Notes
- Data sections generally won't match in incomplete files
    - To match string pools in incomplete files (since they affect asm), functions with strings that haven't been decompiled have temporary replacements that call a fake __dummy_string function to place these strings in the pool
    - To match float pools in incomplete rel files, __dummy_float is used

## TTYD Support
TTYD was initially matched too for some of seqdrv, but this was abandoned based on the effort it would've taken to keep doing and also to find the right compiler version & flags for it too. The code that was matched still remains.

## Credits
- Various members of the TTYD community for their [documentation](https://github.com/PistonMiner/ttyd-tools) and for porting the demo symbol map to the final game
- The [PM64 decomp](https://github.com/ethteck/papermario) team for decompiling many of the same functions, which are useful to check against
