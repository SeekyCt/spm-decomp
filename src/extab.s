/*
    Pointers in extab to static functions will break, and extab can't compile matching
    currently, so this workaround is used. Will need fixing for shiftability(?).
*/

.section extab_, "a"

    .incbin "main.dol", 0x300a80, 0x95e0

.section extabindex_, "a"

.global lbl_8000fac0
lbl_8000fac0:
    .incbin "main.dol", 0x30a060, 0xdf74

.global lbl_8001da34
lbl_8001da34:
    .incbin "main.dol", 0x317fd4, 0x2c
