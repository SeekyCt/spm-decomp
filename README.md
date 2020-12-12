# spm-decomp

Messy decompilation of Super Paper Mario, I'd planned to clean this up and make it public but it's been months and I still haven't so I might as well just release what I have; I'm not really motivated to work on this any more anyway. This can't build a full dol or anything, code was compared by compiling with '-S'.

Development was done with Codewarrior Version 4.1 build 60831, which I think is a slightly wrong version. The main reason is that I've often had to use while loops with a bunch of pointers manually incremented instead of indexing by i in a for loop to get the increment order to match, like in this code
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

Early code also aimed to match TTYD concurrently; I gave up on this after seqdrv, but what I did is still in there.

## Credits
Various members of the TTYD community for their documentation here https://github.com/PistonMiner/ttyd-tools and for porting the demo symbol map to the final game
