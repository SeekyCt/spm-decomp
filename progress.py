"""
Progress message generation
"""

import os.path
import pickle
import json
import common as c

# TODO: other sections than .text

def print_progress(ctx: c.SourceContext, asm_list: str, fmt: str):
    assert os.path.exists(ctx.labels), "Error: analysis has not ran!"

    # Get data
    raw = c.get_cmd_stdout(f"{c.PROGRESS} {ctx.binary} {ctx.labels} {ctx.slices}")
    dat = json.loads(raw)
    decomp_size = dat["decomp_slices_size"]
    total_size = dat["total_size"]
    func_sizes = dat["symbol_sizes"]

    # Subtract undecompiled functions in decompiled slices
    with open(asm_list, 'rb') as f:
        funcs = pickle.load(f)
    for func in funcs:
        decomp_size -= func_sizes[func]
    
    # Output
    print(
        fmt.format(
            d=hex(decomp_size),
            t=hex(total_size),
            p=(decomp_size/total_size)*100
        )
    )

print_progress(c.DOL_CTX, c.DOL_ASM_LIST,
               "main.dol .text section progress: {d}/{t} bytes ({p:.4f}%)")
print_progress(c.REL_CTX, c.REL_ASM_LIST,
               "relF.rel .text section progress: {d}/{t} bytes ({p:.4f}%)")
