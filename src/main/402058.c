#include "common.h"
#include "pad.h"
#include "sys.h"
#include "ptcl.h"

void DbgPtclEditTrack4(PtclCtrl *arg0, PtclTrack *arg1) {
    Pad *pad = padPort0;
    s16 max;
    s32 n;

    if (pad[1].repeat & 0x2000) {
        sysState.dbgDigit++;
    } else if (pad[1].repeat & 0x8000) {
        sysState.dbgDigit--;
    }
    if (sysState.dbgDigit <= 0) {
        sysState.dbgDigit = 1;
    } else if (sysState.dbgDigit >= 8) {
        sysState.dbgDigit = 7;
    }
    n = sysState.dbgDigit;
    if (n & 1) {
        if (pad[1].repeat & 0x1000) {
            arg1->time[n >> 1]++;
        } else if (pad[1].repeat & 0x4000) {
            arg1->time[n >> 1]--;
        }
    } else {
        if (pad[1].repeat & 0x1000) {
            arg1->curve[(n >> 1) - 1]++;
        } else if (pad[1].repeat & 0x4000) {
            arg1->curve[(n >> 1) - 1]--;
        }
    }
    max = arg1->time[0];
    arg1->lastKey = 0;
    if (max < arg1->time[1]) {
        max = arg1->time[1];
        arg1->lastKey = 1;
    }
    if (max < arg1->time[2]) {
        max = arg1->time[2];
        arg1->lastKey = 2;
    }
    if (max < arg1->time[3]) {
        max = arg1->time[3];
        arg1->lastKey = 3;
    }
    if (arg0->length < max) {
        arg0->length = max;
    }
}
