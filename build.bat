set dolflags=-O4 -proc gekko -lang C99 -S -w all -use_lmw_stmw on -fp hard -str pool -rostr -I- -i include -enc SJIS -inline all -sdata2 4

mwcceppc %dolflags% src/evtmgr.c
mwcceppc %dolflags% src/evtmgr_cmd.c
mwcceppc %dolflags% src/filemgr.c
mwcceppc %dolflags% src/main.c
mwcceppc %dolflags% src/mario_pouch.c
mwcceppc %dolflags% src/memory.c
mwcceppc %dolflags% src/nandmgr.c
mwcceppc %dolflags% src/relmgr.c
mwcceppc %dolflags% src/seqdef.c
mwcceppc %dolflags% src/seqdrv.c
mwcceppc %dolflags% src/spmario.c
mwcceppc %dolflags% src/swdrv.c
mwcceppc %dolflags% src/system.c
mwcceppc %dolflags% src/windowdrv.c
mwcceppc %dolflags% src/wpadmgr.c

set relflags=-O4 -proc gekko -lang C99 -S -w all -use_lmw_stmw on -fp hard -str pool -rostr -I- -i include -enc SJIS -inline all -sdata 0 -sdata2 0 -pool off -ordered-fp-compares

mwcceppc %relflags% rel/dan.c
