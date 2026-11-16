#include "common.h"
#include "pad.h"
#include "sys.h"

void DbgBtlPageSelect(Pad *pad, s32 max) {
    if (pad->trigger & 4) {
        if (--sysState.dbgPage < 0) {
            sysState.dbgPage = max;
        }
        sysState.dbgCursor = 0;
    } else if (pad->trigger & 8) {
        if (++sysState.dbgPage > max) {
            sysState.dbgPage = 0;
        }
        sysState.dbgCursor = 0;
    }
}
