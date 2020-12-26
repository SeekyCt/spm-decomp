#include <common.h>
#include <evtmgr_cmd.h>
#include <evtmgr.h>
#include <stdio.h>
#include <string.h>
#include <swdrv.h>
#include <system.h>

static float check_float(s32 val) { // always inlined
    if (val <= EVTDAT_FLOAT_MAX) {
        return (val + EVTDAT_FLOAT_BASE) / 1024.0f;
    }
    else {
        return val;
    }
}

// change_float (inlined/unused)

EVT_CMD_FN(end_evt) {
    evtDelete(entry);
    return EVT_END;
}

EVT_CMD_FN(lbl) {
    (void) entry;
    return EVT_CONTINUE;
}

// Matching but not good
EVT_CMD_FN(goto) {
    s32 lbl = evtGetValue(entry, entry->pCurData[0]);
    EvtScriptCode * r31 = entry->pCurData;
    EvtScriptCode * dest;
    if (lbl < EVTDAT_ADDR_MAX) { // TODO: evtSearchLabel inlined
        dest = (EvtScriptCode *) lbl;
    }
    else {
        s32 n;
        for (n = 0; n < MAX_EVT_JMPTBL; n++) {
            if (lbl == entry->labelIds[n]) {
                r31 = entry->jumptable[n];
                break;
            }
        }
        if (n >= MAX_EVT_JMPTBL) {
            assertf(0, "EVTMGR_CMD:Jump Table Search error !!\n [lbl=%d, n=%d]", lbl, n);
        }
        dest = r31;
    }
    entry->pCurInstruction = dest;
    return EVT_CONTINUE;
}

EVT_CMD_FN(do) {
    EvtScriptCode * p = entry->pCurData;
    s32 count = *p++;
    s32 depth = ++entry->dowhileDepth; // missing rlwinm here + scheduling problems
    if (depth >= 8) {
        assert(0, "EVTMGR_CMD:While Table Overflow !!");
    }
    entry->dowhileStartPtrs[depth] = p;
    entry->dowhileCounters[depth] = count;
    return EVT_CONTINUE;
}

EVT_CMD_FN(while) {
    s32 count; // only way the register usage of depth's sign extend matched
    s32 depth = entry->dowhileDepth;
    if (depth < 0) {
        assert(0, "EVTMGR_CMD:While Table Underflow !!");
    }
    count = entry->dowhileCounters[depth];
    if (count == 0) {
        entry->pCurInstruction = entry->dowhileStartPtrs[depth];
        return EVT_CONTINUE;
    }
    else {
        if (count >= -10000000) {
            entry->dowhileCounters[depth] = --count;
        }
        else {
            s32 ret = evtGetValue(entry, count);
            evtSetValue(entry, count, ret - 1);
            count = ret - 1;
        }
        if (count != 0) {
            entry->pCurInstruction = entry->dowhileStartPtrs[depth];
            return EVT_CONTINUE;
        }
        else {
            entry->dowhileDepth--;
            return EVT_CONTINUE;
        }
    }
}

EVT_CMD_FN(do_break) {
    if (entry->dowhileDepth < 0) {
        assert(0, "EVTMGR_CMD:While Table Underflow !!");
    }
    entry->pCurInstruction = evtSearchWhile(entry);
    entry->dowhileDepth -= 1;
    return EVT_CONTINUE;
}

EVT_CMD_FN(do_continue) {
    if (entry->dowhileDepth < 0) {
        assert(0, "EVTMGR_CMD:While Table Underflow !!");
    }
    entry->pCurInstruction = evtSearchJustBeforeWhile(entry);
    return EVT_CONTINUE;
}

EVT_CMD_FN(wait_frm) {
    EvtScriptCode * p = entry->pCurData;
    if (!entry->blocked) { // wrong branch setup here
        entry->unknown_0x74 = evtGetValue(entry, *p);
        entry->blocked = 1;
    }

    if (entry->unknown_0x74 == 0) {
        return EVT_CONTINUE;
    }
    else {
        return !--entry->unknown_0x74;
    }
}

// EVT_CMD_FN(wait_msec)

EVT_CMD_FN(halt) {
   return evtGetValue(entry, *entry->pCurData) ? 0 : EVT_CONTINUE;
}

EVT_CMD_FN(if_str_equal) {
    EvtScriptCode * p = entry->pCurData;
    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);
    if (s1 == NULL) {
        s1 = "";
    }
    if (s2 == NULL) {
        s2 = "";
    }
    if (strcmp(s1, s2)) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(if_str_not_equal) {
    EvtScriptCode * p = entry->pCurData;
    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);
    if (s1 == NULL) {
        s1 = "";
    }
    if (s2 == NULL) {
        s2 = "";
    }
    if (!strcmp(s1, s2)) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(if_str_small) {
    EvtScriptCode * p = entry->pCurData;
    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);
    if (s1 == NULL) {
        s1 = "";
    }
    if (s2 == NULL) {
        s2 = "";
    }
    if (strcmp(s1, s2) >= 0) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(if_str_large) {
    EvtScriptCode * p = entry->pCurData;
    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);
    if (s1 == NULL) {
        s1 = "";
    }
    if (s2 == NULL) {
        s2 = "";
    }
    if (strcmp(s1, s2) <= 0) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}


EVT_CMD_FN(if_str_small_equal) {
    EvtScriptCode * p = entry->pCurData;
    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);
    if (s1 == NULL) {
        s1 = "";
    }
    if (s2 == NULL) {
        s2 = "";
    }
    if (strcmp(s1, s2) > 0) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(if_str_large_equal)  {
    EvtScriptCode * p = entry->pCurData;
    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);
    if (s1 == NULL) {
        s1 = "";
    }
    if (s2 == NULL) {
        s2 = "";
    }
    if (strcmp(s1, s2) < 0) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(iff_equal) {
    EvtScriptCode * p = entry->pCurData;
    float f1 = evtGetFloat(entry, p[0]);
    float f2 = evtGetFloat(entry, p[1]);
    if (f1 != f2) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(iff_not_equal) {
    EvtScriptCode * p = entry->pCurData;
    f32 f1 = evtGetFloat(entry, p[0]);
    f32 f2 = evtGetFloat(entry, p[1]);
    if (f1 == f2) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(iff_small) {
    EvtScriptCode * p = entry->pCurData;
    f32 f1 = evtGetFloat(entry, p[0]);
    f32 f2 = evtGetFloat(entry, p[1]);
    if (f1 >= f2) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(iff_large) {
    EvtScriptCode * p = entry->pCurData;
    f32 f1 = evtGetFloat(entry, p[0]);
    f32 f2 = evtGetFloat(entry, p[1]);
    if (f1 <= f2) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(iff_small_equal) {
    EvtScriptCode * p = entry->pCurData;
    f32 f1 = evtGetFloat(entry, p[0]);
    f32 f2 = evtGetFloat(entry, p[1]);
    if (f1 > f2) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(iff_large_equal) {
    EvtScriptCode * p = entry->pCurData;
    f32 f1 = evtGetFloat(entry, p[0]);
    f32 f2 = evtGetFloat(entry, p[1]);
    if (f1 < f2) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(if_equal) {
    EvtScriptCode * p = entry->pCurData;
    s32 val1 = evtGetValue(entry, p[0]);
    s32 val2 = evtGetValue(entry, p[1]);
    if (val1 != val2) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(if_not_equal) {
    EvtScriptCode * p = entry->pCurData;
    s32 val1 = evtGetValue(entry, p[0]);
    s32 val2 = evtGetValue(entry, p[1]);
    if (val1 == val2) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

// EVT_CMD_FN(if_small)
// EVT_CMD_FN(if_large)
// EVT_CMD_FN(if_small_equal)
// EVT_CMD_FN(if_large_equal)


EVT_CMD_FN(if_flag) {
    EvtScriptCode * p = entry->pCurData;
    s32 val = evtGetValue(entry, p[0]);
    s32 mask = p[1];
    if ((val & mask) == 0) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(if_not_flag) {
    EvtScriptCode * p = entry->pCurData;
    s32 val = evtGetValue(entry, p[0]);
    s32 mask = p[1];
    if ((val & mask) != 0) {
        entry->pCurInstruction = evtSearchElse(entry);
        return EVT_CONTINUE;
    }
    return EVT_CONTINUE;
}

EVT_CMD_FN(else) {
    entry->pCurInstruction = evtSearchEndIf(entry);
    return EVT_CONTINUE;
}

EVT_CMD_FN(end_if) {
    (void) entry;
    return EVT_CONTINUE;
}

// EVT_CMD_FN(switch)
// EVT_CMD_FN(switchi)
// EVT_CMD_FN(case_equal)
// EVT_CMD_FN(case_not_equal)
// EVT_CMD_FN(case_small)
// EVT_CMD_FN(case_small_equal)
// EVT_CMD_FN(case_large)
// EVT_CMD_FN(case_large_equal)
// EVT_CMD_FN(case_between)
// EVT_CMD_FN(case_etc)
// EVT_CMD_FN(case_flag)
// EVT_CMD_FN(case_or)
// EVT_CMD_FN(case_and)
// EVT_CMD_FN(case_end)
// EVT_CMD_FN(switch_break)
// EVT_CMD_FN(end_switch)
// EVT_CMD_FN(set)
// EVT_CMD_FN(seti)
// EVT_CMD_FN(setf)
// EVT_CMD_FN(add)
// EVT_CMD_FN(sub)
// EVT_CMD_FN(mul)
// EVT_CMD_FN(div)
// EVT_CMD_FN(mod)
// EVT_CMD_FN(addf)
// EVT_CMD_FN(subf)
// EVT_CMD_FN(mulf)
// EVT_CMD_FN(divf)
// EVT_CMD_FN(set_read)
// EVT_CMD_FN(set_readf)
// EVT_CMD_FN(read)
// EVT_CMD_FN(read2)
// EVT_CMD_FN(read3)
// EVT_CMD_FN(read4)
// EVT_CMD_FN(read_n)
// EVT_CMD_FN(readf)
// EVT_CMD_FN(readf2)
// EVT_CMD_FN(readf3)
// EVT_CMD_FN(readf4)
// EVT_CMD_FN(readf_n)

EVT_CMD_FN(clamp_int) {
    s32 * pData = entry->pCurData;

    s32 dest = pData[0];
    s32 destVal = evtGetValue(entry, dest);
    s32 minVal = evtGetValue(entry, pData[1]);
    s32 maxVal = evtGetValue(entry, pData[2]);

    if (destVal < minVal) {
        evtSetValue(entry, dest, minVal);
    }

    if (destVal > maxVal) {
        evtSetValue(entry, dest, maxVal);
    }
    
    return EVT_CONTINUE;
}

// EVT_CMD_FN(set_user_wrk)
// EVT_CMD_FN(set_user_flg)
// EVT_CMD_FN(alloc_user_wrk)
// EVT_CMD_FN(and)
// EVT_CMD_FN(andi)
// EVT_CMD_FN(or)
// EVT_CMD_FN(ori)
// EVT_CMD_FN(set_frame_from_msec)
// EVT_CMD_FN(set_msec_from_frame)
// EVT_CMD_FN(set_ram)
// EVT_CMD_FN(set_ramf)
// EVT_CMD_FN(get_ram)
// EVT_CMD_FN(get_ramf)
// EVT_CMD_FN(setr)
// EVT_CMD_FN(setrf)
// EVT_CMD_FN(getr)
// EVT_CMD_FN(getrf)
// EVT_CMD_FN(user_func)
// EVT_CMD_FN(run_evt)
// EVT_CMD_FN(run_evt_id)
// EVT_CMD_FN(run_child_evt)
// EVT_CMD_FN(restart_evt)
// EVT_CMD_FN(delete_evt)
// EVT_CMD_FN(set_pri)
// EVT_CMD_FN(set_spd)
// EVT_CMD_FN(set_type)
// EVT_CMD_FN(stop_all)
// EVT_CMD_FN(start_all)
// EVT_CMD_FN(stop_other)
// EVT_CMD_FN(start_other)
// EVT_CMD_FN(stop_id)
// EVT_CMD_FN(start_id)
// EVT_CMD_FN(chk_evt)
// EVT_CMD_FN(inline_evt)
// EVT_CMD_FN(inline_evt_id)
// EVT_CMD_FN(end_inline)
// EVT_CMD_FN(brother_evt)
// EVT_CMD_FN(brother_evt_id)
// EVT_CMD_FN(end_brother)

EVT_CMD_FN(debug_put_msg) {
    evtGetValue(entry, *entry->pCurData);
    return EVT_CONTINUE;
}

EVT_CMD_FN(debug_msg_clear) {
    (void) entry;
    return EVT_CONTINUE;
}

EVT_CMD_FN(debug_put_reg) {
    static char str[256]; // 8050ce30
    EvtScriptCode * p = entry->pCurData;
    EvtWork * wp = evtGetWork();
    s32 reg = *p;
    if (reg <= EVTDAT_ADDR_MAX) {
        sprintf(str, "ADDR     [%08X]", reg);
    }
    else if (reg <= EVTDAT_FLOAT_MAX) {
        f32 f = check_float(reg);
        sprintf(str, "FLOAT    [%4.2f]", f);
    }
    else if (reg <= EVTDAT_UF_MAX) {
        reg += EVTDAT_UF_BASE;
        u32 mask = 1U << (reg % 32);
        u32 dat = entry->uf[reg / 32];
        sprintf(str, "UF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_UW_MAX) {
        reg += EVTDAT_UW_BASE;
        s32 val = entry->uw[reg];
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(str, "UW(%3d)  [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            f32 f = check_float(val);
            sprintf(str, "UW(%3d)  [%4.2f]", reg, f);
        }
        else {
            sprintf(str, "UW(%3d)  [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_GSW_MAX) {
        reg += EVTDAT_GSW_BASE;
        s32 val = swByteGet(reg);
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(str, "GSW(%3d) [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            f32 f = check_float(val);
            sprintf(str, "GSW(%3d) [%4.2f]", reg, f);
        }
        else {
            sprintf(str, "GSW(%3d) [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_LSW_MAX) {
        reg += EVTDAT_LSW_BASE;
        s32 val = _swByteGet(reg);
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(str, "LSW(%3d) [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            f32 f = check_float(val);
            sprintf(str, "LSW(%3d)  [%4.2f]", reg, f);
        }
        else {
            sprintf(str, "LSW(%3d) [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_GSWF_MAX) {
        reg += EVTDAT_GSWF_BASE;
        sprintf(str, "GSWF(%3d)[%d]", reg, swGet(reg));
    }
    else if (reg <= EVTDAT_LSWF_MAX) {
        reg += EVTDAT_LSWF_BASE;
        sprintf(str, "LSWF(%3d)[%d]", reg, _swGet(reg));
    }
    else if (reg <= EVTDAT_GF_MAX) {
        reg += EVTDAT_GF_BASE;
        u32 mask = 1U << (reg % 32);
        u32 dat = wp->gf[reg / 32];
        sprintf(str, "GF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_LF_MAX) {
        reg += EVTDAT_LF_BASE;
        u32 mask = 1U << (reg % 32);
        u32 dat = entry->lf[reg / 32];
        sprintf(str, "LF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_GW_MAX) {
        reg += EVTDAT_GW_BASE;
        s32 val = wp->gw[reg];
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(str, "GW(%3d)  [%08X]", reg, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            f32 f = check_float(val);
            sprintf(str, "GW(%3d)  [%4.2f]", reg, f);
        }
        else {
            sprintf(str, "GW(%3d)  [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_LW_MAX) {
        reg += EVTDAT_LW_BASE;
        s32 val = entry->lw[reg];
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(str, "LW(%3d)  [%08X]", reg, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            f32 f = check_float(val);
            sprintf(str, "LW(%3d)  [%4.2f]", reg, f);
        }
        else {
            sprintf(str, "LW(%3d)  [%d]", reg, val);
        }
    }
    else {
        sprintf(str, "         [%d]", reg);
    }

    return EVT_CONTINUE;
}

EVT_CMD_FN(debug_name) {
    entry->name = (char *) *entry->pCurData;
    return EVT_CONTINUE;
}

EVT_CMD_FN(debug_rem) {
    (void) entry;
    return EVT_CONTINUE;
}

EVT_CMD_FN(debug_bp) {
    for (s32 i = 0; i < EVT_ENTRY_MAX; i++) {
        if (entry == evtGetPtr(i)) break;
    }
    return 1;
}

// evtmgrCmd
// evtGetValue
// evtGetNumber (inlined/unused)
// evtSetValue
// evtGetFloat
// evtSetFloat

// evtSearchLabel (inlined)

EvtScriptCode * evtSearchElse(EvtEntry * entry) {
    s32 ifDepth = 0;
    EvtScriptCode * pInstr = entry->pCurInstruction;
    while (true) {
        s32 opc = *pInstr & 0xffff;
        pInstr += *pInstr++ >> 16;
        switch (opc) {
            case EVT_OPC_END_SCRIPT:
                assert(0, "EVTMGR_CMD:'ELSE' Search Error !!");
            case EVT_OPC_END_IF:
                if (--ifDepth >= 0) break;
                else return pInstr;
            case EVT_OPC_IF_STR_EQUAL ... EVT_OPC_IF_NOT_FLAG:
                ifDepth += 1;
                break;
             case EVT_OPC_ELSE:
                if (ifDepth == 0) return pInstr;
                else break;
       }
    }
}

EvtScriptCode * evtSearchEndIf(EvtEntry * entry) {
    s32 ifDepth = 0;
    EvtScriptCode * pInstr = entry->pCurInstruction;
    while (true) {
        s32 opc = *pInstr & 0xffff;
        pInstr += *pInstr++ >> 16;
        switch (opc) {
            case EVT_OPC_END_SCRIPT:
                assert(0, "EVTMGR_CMD:'END_IF' Search Error !!");
            case EVT_OPC_END_IF:
                if (--ifDepth >= 0) break;
                else return pInstr;
            case EVT_OPC_IF_STR_EQUAL ... EVT_OPC_IF_NOT_FLAG:
                ifDepth += 1;
                break;
       }
    }
}

EvtScriptCode * evtSearchEndSwitch(EvtEntry * entry) {
    EvtScriptCode * pInstr = entry->pCurInstruction;
    s32 switchDepth = 1;
    while (true) {
        s32 opc = *pInstr & 0xffff;
        EvtScriptCode * ret = pInstr;
        pInstr += *pInstr++ >> 16;
        switch(opc) {
            case EVT_OPC_END_SCRIPT:
                assert(0, "EVTMGR_CMD:'END_SWITCH' Search Error !!");
            case EVT_OPC_SWITCH:
                switchDepth += 1;
                break;
            case EVT_OPC_END_SWITCH:
                if (--switchDepth == 0) return ret;
                else break;
        }
    }
}

EvtScriptCode * evtSearchCase(EvtEntry * entry) {
    EvtScriptCode * pInstr = entry->pCurInstruction;
    s32 switchDepth = 1;
    while (true) {
        s32 opc = *pInstr & 0xffff;
        EvtScriptCode * ret = pInstr;
        pInstr += *pInstr++ >> 16;
        switch(opc) {
            case EVT_OPC_END_SCRIPT:
                assert(0, "EVTMGR_CMD:'CASE' Search Error !!");
            case EVT_OPC_SWITCH:
                switchDepth += 1;
                break;
            case EVT_OPC_END_SWITCH:
                if (--switchDepth == 0) return ret;
                else break;
            case EVT_OPC_CASE_EQUAL ... EVT_OPC_CASE_BETWEEN:
                if (switchDepth == 1) return ret;
                else break;
        }
    }
}

EvtScriptCode * evtSearchWhile(EvtEntry * entry) {
    s32 dowhileDepth = 0;
    EvtScriptCode * pInstr = entry->pCurInstruction;
    while (true) {
        s32 opc = *pInstr & 0xffff;
        pInstr += *pInstr++ >> 16;
        switch (opc) {
            case EVT_OPC_END_SCRIPT:
                assert(0, "EVTMGR_CMD:'WHILE' Search Error !!");
            case EVT_OPC_WHILE:
                if (--dowhileDepth >= 0) break;
                else return pInstr;
            case EVT_OPC_DO:
                dowhileDepth += 1;
                break;
        }
    }
}

EvtScriptCode * evtSearchJustBeforeWhile(EvtEntry * entry) {
    s32 dowhileDepth = 0;
    EvtScriptCode * pInstr = entry->pCurInstruction;
    while (true) {
        s32 opc = *pInstr & 0xffff;
        switch (opc) {
            case EVT_OPC_END_SCRIPT:
                assert(0, "EVTMGR_CMD:'WHILE' Search Error !!");
            case EVT_OPC_WHILE:
                if (--dowhileDepth >= 0) break;
                else return pInstr;
            case EVT_OPC_DO:
                dowhileDepth += 1;
                break;
        }
        pInstr += *pInstr++ >> 16;
    }
}
