#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McLoadSelectPort(void) {
    s32 done;
    u16 k;

    /* do-while(0): keeps the block layout and the aligned-label nop of the port loop */
    do {
        done = 0;
        McSetPollInterval(0x1E);
        mcLoadPortCursor = mcLoadPort;
        if (mcLoadStateChanged != 0) {
            goto clamp;
        }
        if (mcLoadPad->trigger & 0x5000) {
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        }
        k = mcLoadPad->trigger;
        if (k & 0x4000) {
            mcLoadPortCursor++;
            goto clamp;
        }
        if (k & 0x1000) {
            mcLoadPortCursor--;
            goto clamp;
        }
        if (k & 0x20) {
            if (mcLoadPortCursor == 0 && (mcCardInfo0->state < 2 || mcCardInfo0->type != 2)) {
                SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                goto clamp;
            }
            if (mcLoadPortCursor == 1 && (mcCardInfo1->state < 2 || mcCardInfo1->type != 2)) {
                SndSePlay(0x7D4, 0, 0, 0x1E, 0);
                goto clamp;
            }
            done = 1;
            McLoadSetState(0x100);
            mcLoadChoice = mcLoadPortCursor;
            McSelectPort(mcLoadPortCursor);
            SndSePlay(0x7D2, 0, 0, 0x1E, 0);
        }
        if (mcLoadPad->trigger & 0x40) {
            done = 1;
            McLoadSetState(0x801);
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
        }
    clamp:
        if (mcLoadPortCursor < 0) {
            mcLoadPortCursor = 1;
        }
        if (mcLoadPortCursor >= 2) {
            mcLoadPortCursor = 0;
        }
        mcLoadPort = mcLoadPortCursor;
        mcLoadPortTimer--;
        /* do-while(0): loop-depth weight that sets the s-register order */
        do {
            if (mcLoadStateChanged != 0) {
                FldMenuMcDlgSlotSelect(0, mcLoadPortCursor);
            } else if (done) {
                FldMenuMcDlgSlotSelect(2, mcLoadPortCursor);
            } else {
                FldMenuMcDlgSlotSelect(1, mcLoadPortCursor);
            }
        } while (0);
    } while (0);
}
