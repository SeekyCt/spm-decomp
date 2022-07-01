/*
    These behave weirdly, and seemingly inconsistently between the dol & the
    rel so this hasn't been implemented in the disassembler itself.

    For ctors:
    - The compiler automatically adds a 0 word, so the section is left empty

    For dtors:
    - Directly after __destroy_global_chain_reference another pointer to
      __destroy_global_chain pointer is inserted, so that's omitted here
    - A null pointer dtors$99 is added after all other data, so that's
      omitted here too
*/

.include "macros.inc"

.section .dtors

.global __destroy_global_chain_reference
__destroy_global_chain_reference:
/* 80CA9ADC 80C83FF0 */ .4byte      __destroy_global_chain
