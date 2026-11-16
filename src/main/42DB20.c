#include "common.h"
#include "pad.h"


void PadUpdateRepeat(Pad *arg0) {
    s32 i;
    s32 j;
    s32 bit;
    u16 old;

    for (i = 0; i < 2; i++) {
        switch (padPhase[i]) {
        case 0:
        case 0x28:
        case 0x29:
        case 0x2A:
        case 0x46:
        case 0x47:
        case 0x4C:
        case 0x4D:
            continue;
        }

        old = arg0[i].held;
        arg0[i].repeat = old;
        arg0[i].repeatFast = arg0[i].held;
        arg0[i].released = 0;
        for (j = 0; j < 16; j++) {
            bit = 1 << j;
            if ((arg0[i].held & bit) > 0 && (arg0[i].held & bit) == (old & bit)) {
                if (arg0[i].repeatStarted[j] == 1) {
                    if (arg0[i].repeatCount[j] < 8) {
                        arg0[i].repeat ^= bit;
                        arg0[i].repeatCount[j]++;
                    } else {
                        arg0[i].repeatCount[j] = 0;
                    }
                } else if (arg0[i].repeatCount[j] < 15) {
                    arg0[i].repeat ^= bit;
                    arg0[i].repeatCount[j]++;
                } else {
                    arg0[i].repeatCount[j] = 0;
                    arg0[i].repeatStarted[j] = 1;
                }
            } else {
                arg0[i].repeatStarted[j] = 0;
                arg0[i].repeatCount[j] = 0;
            }
            if ((arg0[i].held & bit) > 0 && (arg0[i].held & bit) == (old & bit)) {
                if (arg0[i].repeatFastCount[j] < 15) {
                    arg0[i].repeatFast ^= bit;
                    arg0[i].repeatFastCount[j]++;
                }
            } else {
                arg0[i].repeatFastCount[j] = 0;
            }
            if ((arg0[i].prevHeld & bit) && !(arg0[i].held & bit)) {
                arg0[i].released |= bit;
            }
        }
    }
}
