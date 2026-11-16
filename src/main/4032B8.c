/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "sys.h"
#include "ptcl.h"
#include "main/n01.h"

extern s32 dbgPtclCtrlIndex;
extern u8 dbgPtclCtrlTitleFmt[];

void DbgPtclCtrlPage(void) {
    u8 *ent;
    s32 *p;
    Pad *pad;
    s32 v;
    s32 r;

    if (ptclPtbData == NULL) {
        return;
    }
    ent = (u8 *)ptclPtbData + ptclPtbData->ctrlOffset + dbgPtclCtrlIndex * 0x770;
    if ((sysState.dbgMenuFlags & 0xF000) == 0x1000) {
        DbgPtclCtrlGenPage(ent);
        return;
    }
    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        if (pad->trigger & 4) {
            if (--sysState.dbgPage < 0) {
                sysState.dbgPage = 3;
            }
            sysState.dbgCursor = 0;
        } else if (pad->trigger & 8) {
            if (++sysState.dbgPage >= 4) {
                sysState.dbgPage = 0;
            }
            sysState.dbgCursor = 0;
        }
        if (sysState.dbgCursor == 0) {
            p = &dbgPtclCtrlIndex;
            v = dbgPtclCtrlIndex + DbgEditValue(0, 0, 3, 1.0f);
            dbgPtclCtrlIndex = v;
            if (v < 0) {
                r = ptclPtbData->ctrlCount - 1;
            } else {
                r = (ptclPtbData->ctrlCount - 1 < v) ? 0 : v;
            }
            *p = r;
            ent = (u8 *)ptclPtbData + ptclPtbData->ctrlOffset + dbgPtclCtrlIndex * 0x770;
            if (ent != NULL && (pad->repeat & 0x20)) {
                if (PtclStarterKill(0) != 0) {
                    PtclStarterSpawn(0, dbgPtclCtrlIndex, 0);
                }
            }
        }
    }
    switch (sysState.dbgPage) {
    case 0:
        DbgPtclCtrlFlagsPage(ent);
        break;
    case 1:
        DbgPtclCtrlEmitTrackPage(ent);
        break;
    case 2:
        DbgPtclCtrlScaleTrackPage(ent);
        break;
    case 3:
        DbgPtclCtrlRotTrackPage(ent);
        break;
    }
    DbgPrintf(3, 2, dbgPtclCtrlTitleFmt, dbgPtclCtrlIndex, dbgPtclTitleColorStr, sysState.dbgPage + 1, dbgPtclColor0Str);
}
