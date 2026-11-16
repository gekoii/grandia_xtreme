#include "common.h"
#include "pad.h"
#include "sys.h"

void DbgBtlLineSelect(Pad *pad, s32 max) {
    if (!(pad->held & 0x20)) {
        u16 b = pad->repeat;
        if (b & 0x1000) {
            if (--sysState.dbgCursor < 0) {
                sysState.dbgCursor = max;
            }
        } else if (b & 0x4000) {
            if (++sysState.dbgCursor > max) {
                sysState.dbgCursor = 0;
            }
        }
    }
}
