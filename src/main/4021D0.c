#include "common.h"
#include "pad.h"
#include "sys.h"
#include "ptcl.h"

void DbgPtclEditTrack3(PtclCtrl *arg0, PtclStep *arg1) {
    Pad *pad = padPort0;

    s16 max;

    if (pad[1].repeat & 0x2000) {
        sysState.dbgDigit++;
    } else if (pad[1].repeat & 0x8000) {
        sysState.dbgDigit--;
    }
    if (sysState.dbgDigit <= 0) {
        sysState.dbgDigit = 1;
    } else if (sysState.dbgDigit >= 4) {
        sysState.dbgDigit = 3;
    }
    if (pad[1].repeat & 0x1000) {
        arg1->time[sysState.dbgDigit - 1]++;
    } else if (pad[1].repeat & 0x4000) {
        arg1->time[sysState.dbgDigit - 1]--;
    }
    max = arg1->time[0];
    arg1->on = 1;
    if (max < arg1->time[1]) {
        max = arg1->time[1];
        arg1->on = 2;
    }
    if (max < arg1->time[2]) {
        max = arg1->time[2];
        arg1->on = 3;
    }
    if (max <= 0) {
        arg1->on = 0;
        return;
    }
    if (arg0->length < max) {
        arg0->length = max;
    }
}
