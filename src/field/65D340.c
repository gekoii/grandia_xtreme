#include "common.h"
#include "field/n26.h"
#include "protos/field.h"


void McSaveSelectPort(void) {
    s32 done;
    s8 sel;
    u16 btn;
    Pad *p;

    done = 0;
    McSetPollInterval(0x3C);
    sel = mcSavePort;
    /* empty do-while(0): scheduling barrier, the setup above stays ahead of the pad tests */
    do {} while (0);
    if (mcSaveStateChanged == 0) {
        p = mcSavePad;
        if (p->trigger & 0x5000) {
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            p = mcSavePad;
        }
        goto check;
    body:
        if (btn & 0x1000) {
            sel--;
            goto clamp;
        }
        if (btn & 0x20) {
            if ((sel == 0 && (mcCardInfo0->state < 2 || mcCardInfo0->type != 2)) ||
                (sel == 1 && (mcCardInfo1->state < 2 || mcCardInfo1->type != 2))) {
                /* cast call: a plain SndSePlay here changes the block layout (6 instructions fewer) */
                ((s64 (*)(s32, s32, s32, s32, s32))SndSePlay)(0x7D4, 0, 0, 0x1E, 0);
                goto clamp;
            }
            done = 1;
            McSaveSetState(0x100);
            mcSaveChoice = sel;
            McSelectPort(sel);
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
        }
        if (mcSavePad->trigger & 0x40) {
            if (EvtSystemFlagGet(0x96) != 0) {
                if (mcSaveDone != 0) {
                    McSaveSetState(0x800);
                }
            } else {
                if (mcSaveDone != 0) {
                    McSaveSetState(0x800);
                } else {
                    McSaveSetState(0x30B);
                }
                done = 1;
                SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            }
        }
        goto clamp;
    /* goto loop: the button test is laid out after the body and jumps back into it, as in the original */
    check:
        btn = p->trigger;
        if (!(btn & 0x4000)) {
            goto body;
        }
        sel++;
    }
clamp:
    if (sel < 0) {
        sel = 1;
    }
    if (sel >= 2) {
        sel = 0;
    }
    mcSavePort = sel;
    /* empty do-while(0): scheduling barrier, the mcSavePort store and the loads below stay in this order */
    do {} while (0);
    if (mcSaveStateChanged != 0) {
        /* s64 casts: these calls stay jal (no tail calls) */
        ((s64 (*)(s32, s32))FldMenuMcDlgSlotSelect)(0, sel);
    } else if (done != 0) {
        ((s64 (*)(s32, s32))FldMenuMcDlgSlotSelect)(2, sel);
    } else {
        ((s64 (*)(s32, s32))FldMenuMcDlgSlotSelect)(1, sel);
    }
}
