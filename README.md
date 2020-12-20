# spm-decomp

Decompilation of Super Paper Mario. Doesn't produce a dol, asm is just checked by compiling with -S. TTYD support to some extent.

## Compiler Version Note
The project currently uses Codewarrior Version 4.1 build 60831, which seems to be a slightly wrong version. The main reason that makes this seem like it could be the case is that to match the variable incrementing order in some loops, while loops have had to be used instead of for loops, like in this code
```c
void evtDeleteID(int id) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0 && entry->id == id) {
            evtDelete(entry);
        }
        i++;
        entry++;
    }
}
```
which matches 1:1 in the [Paper Mario 64 decomp](https://github.com/ethteck/papermario/blob/master/src/code_e79b0_len_1920.c#L496) as
```c
void kill_script_by_ID(s32 id) {
    s32 i;
    ScriptInstance* scriptContextPtr;

    for (i = 0; i < MAX_SCRIPTS; i++) {
        scriptContextPtr = (*gCurrentScriptListPtr)[i];
        if (scriptContextPtr != NULL && scriptContextPtr->id == id) {
            kill_script(scriptContextPtr);
        }
    }
}
```
so it seems unlikely the devs would've switched to this weird while format over a for loop.

## Credits
Various members of the TTYD community for their documentation here https://github.com/PistonMiner/ttyd-tools and for porting the demo symbol map to the final game
