#include "common.h"
#include "pad.h"
#include "field/n26.h"
#include "protos/field.h"


s32 McSpaceCheckUpdate(void) {
    s32 ret;
    s32 flag;
    s32 state;
    s32 x;

    flag = 0;
    ret = 0;
    mcSpaceCheckPad = padPort0;
    McSetPollInterval(0x1E);
    if (mcSpaceCheckPrevState != mcSpaceCheckState) {
        mcSpaceCheckStateChanged = 1;
    } else {
        mcSpaceCheckStateChanged = 0;
    }
    state = mcSpaceCheckState;
    switch (state) {
    case 0:
        mcSpaceCheckResult0 = 0;
        mcSpaceCheckResult1 = 1;
        if (mcSpaceCheckStateChanged != 0) {
            McSelectPort(0);
        }
        ret = McSpaceCheckPort();
        if (ret != 0) {
            if (ret < 0) {
                mcSpaceCheckResult0 = ret;
                mcSpaceCheckState = 1;
            }
            if (ret > 0) {
                mcSpaceCheckState = 4;
            }
        }
        break;
    case 1:
        if (mcSpaceCheckStateChanged != 0) {
            McSelectPort(1);
        }
        ret = McSpaceCheckPort();
        if (ret != 0) {
            if (ret < 0) {
                mcSpaceCheckResult1 = ret;
                if (mcSpaceCheckResult0 == -1 && ret == mcSpaceCheckResult0) {
                    mcSpaceCheckState = 3;
                } else {
                    mcSpaceCheckState = 2;
                }
            }
            if (ret > 0) {
                mcSpaceCheckState = 4;
            }
        }
        break;
    case 2:
        if (mcSpaceCheckStateChanged != 0) {
            SndSePlay(0x7D6, 0, 0, 0x1E, 0);
        } else {
            if (mcSpaceCheckPad->trigger & 0x20) {
                ret = 1;
                McSpaceCheckSetState(0);
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                flag = 1;
            }
            if (mcSpaceCheckPad->trigger & 0x40) {
                McSpaceCheckSetState(0);
                flag = 1;
            }
        }
        x = mcSpaceCheckResult0 == -2;
        if (mcSpaceCheckResult1 == -2) {
            x |= 2;
        }
        /* do-while(0): keeps the block order and the aligned-label nop of the dialog calls */
        do {
            if (mcSpaceCheckStateChanged != 0) {
                FldMenuMcDlgCardFull(0, x);
            } else if (flag != 0) {
                FldMenuMcDlgCardFull(2, x);
            } else {
                FldMenuMcDlgCardFull(1, x);
            }
        } while (0);
        break;
    case 3:
        if (mcSpaceCheckStateChanged != 0) {
            SndSePlay(0x7D6, 0, 0, 0x1E, 0);
        } else {
            if (mcSpaceCheckPad->trigger & 0x20) {
                ret = 1;
                McSpaceCheckSetState(0);
                SndSePlay(0x7D2, 0, 0, 0x1E, 0);
                flag = 1;
            }
            if (mcSpaceCheckPad->trigger & 0x40) {
                McSpaceCheckSetState(0);
                flag = 1;
            }
        }
        /* do-while(0): keeps the block order and the aligned-label nop of the dialog calls */
        do {
            if (mcSpaceCheckStateChanged != 0) {
                FldMenuMcDlgNoCard(0);
            } else if (flag != 0) {
                FldMenuMcDlgNoCard(2);
            } else {
                FldMenuMcDlgNoCard(1);
            }
        } while (0);
        break;
    case 4:
        McSpaceCheckSetState(0);
        ret = 1;
        break;
    }
    mcSpaceCheckPrevState = state;
    McSpaceCheckSync();
    return ret;
}
