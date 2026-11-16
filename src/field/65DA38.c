#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McSaveFormat(void) {
    s32 done;
    McPortState *c;
    s32 n;
    McReq *info;

    done = 0;
    if (McGetCurPort()->state == 0) {
        McSaveSetState(0x307);
        return;
    }
    if (mcSaveStateChanged != 0) {
        mcSaveFormatReq = McReqFormat();
        mcSaveFormatStep = 0;
        goto check;
    /* backward gotos from check: the dialog calls are laid out inside the first branch, as in the original */
    l0:
        /* s64 casts: these calls stay jal (no tail calls) */
        ((s64 (*)(s32))FldMenuMcDlgFormatting)(0);
        return;
    l2:
        ((s64 (*)(s32))FldMenuMcDlgFormatting)(2);
        return;
    } else {
        /* do-while(0) as a breakable block: the breaks are its forward exits, laid out as a loop */
        do {
            if (McIsBusy() != 0) {
                break;
            }
            c = McGetCurPort();
            n = McReqGetStatus(mcSaveFormatReq);
            info = McReqGet(mcSaveFormatReq);
            if (n > 0 && info->result >= 0) {
                c->formatted = 1;
                McSaveSetState(0x101);
            } else {
                McSaveSetState(0x307);
            }
            McReqFreeId(mcSaveFormatReq);
            done = 1;
        } while (0);
    }
check:
    if (mcSaveStateChanged != 0) {
        goto l0;
    }
    if (done != 0) {
        goto l2;
    }
    ((s64 (*)(s32))FldMenuMcDlgFormatting)(1);
}
