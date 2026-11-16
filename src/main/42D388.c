#include "common.h"
#include "protos/main.h"
#include "pad.h"

void MemSet(void *, s32, s32);


void PadUpdate(Pad *arg0) {
    s32 i;
    s32 j;
    s32 bit;
    u8 state;
    u16 old;

    for (i = 0; i < 2; i++) {
        state = scePadGetState(i, 0);
        if (state == 0) {
            MemSet(&arg0[i], 0, 0x54);
            arg0[i].modeId = 0;
            arg0[i].leftX = 0x80;
            arg0[i].leftY = 0x80;
            arg0[i].rightX = 0x80;
            arg0[i].rightY = 0x80;
            padPhase[i] = 0;
            continue;
        }
        arg0[i].leftX = 0x80;
        arg0[i].leftY = 0x80;
        arg0[i].rightX = 0x80;
        arg0[i].rightY = 0x80;
        switch (padPhase[i]) {
        case 0:
            arg0[i].pressureMode = 0;
            padModeId[i] = scePadInfoMode(i, 0, 1, 0);
            if (padModeId[i] != 4 && padModeId[i] != 7) {
                padPhase[i] = 0;
                arg0[i].modeId = 0;
                break;
            }
            padExtModeId[i] = scePadInfoMode(i, 0, 2, 0);
            arg0[i].modeId = padModeId[i];
            if (padExtModeId[i] > 0) {
                padModeId[i] = padExtModeId[i];
            }
            switch (padModeId[i]) {
            case 4:
                padPhase[i] = 0x28;
                break;
            case 7:
                padPhase[i] = 0x46;
                break;
            }
            break;
        case 0x28:
            if (scePadInfoMode(i, 0, 2, 0) == 0) {
                padPhase[i] = 0x63;
                break;
            }
            padPhase[i]++;
        case 0x29:
            if (scePadSetMainMode(i, 0, 1, 3) == 1) {
                padPhase[i]++;
            }
            break;
        case 0x2A:
            if (scePadGetState(i, 0) != 5) {
                padPhase[i] = 0;
            }
            break;
        case 0x46:
            if (scePadInfoAct(i, 0, -1, 0) == 0) {
                padPhase[i] = 0x63;
            }
            padActAlign[0] = 0;
            padActAlign[1] = 1;
            for (j = 2; j < 6; j++) {
                padActAlign[j] = 0xFF;
            }
            if (scePadSetActAlign(i, 0, padActAlign) != 0) {
                if (scePadInfoPressMode(i, 0) == 1) {
                    padPhase[i] = 0x4C;
                } else {
                    padPhase[i]++;
                }
            }
            break;
        case 0x47:
            if (scePadGetState(i, 0) != 5) {
                padPhase[i] = 0x63;
            }
            break;
        case 0x4C:
            if (scePadEnterPressMode(i, 0) == 1) {
                padPhase[i]++;
            }
            break;
        case 0x4D:
            if (scePadGetReqState(i, 0) == 1) {
                padPhase[i]--;
            }
            if (scePadGetReqState(i, 0) == 0) {
                arg0[i].pressureMode = 1;
                padPhase[i] = 0x63;
            }
        default:
            old = arg0[i].held;
            if (state != 6 && state != 2) {
                break;
            }
            padType[i] = PadReadPort(i, 0, &arg0[i]);
            if (padType[i] == 0) {
                break;
            }
            if (padLastType[i] != 0 && padType[i] != padLastType[i]) {
                padLastType[i] = 0;
                padPhase[i] = 0;
            }
            if (padModeId[i] == 4) {
                arg0[i].leftX = 0x80;
                arg0[i].leftY = 0x80;
                arg0[i].rightX = 0x80;
                arg0[i].rightY = 0x80;
            }
            if (padModeId[i] == 7) {
                if (arg0[i].vibActive != 0) {
                    if (padVibState.time != 0) {
                        padVibState.time--;
                        padActDirect[1] = padVibState.big;
                        padActDirect[0] = padVibState.small;
                        if (padVibState.big != 0) {
                            padVibState.big -= padVibState.decay;
                            if (padVibState.small == 0 && padVibState.big == 0) {
                                padVibState.time = 0;
                            }
                        }
                        scePadSetActDirect(i, 0, padActDirect);
                    } else {
                        arg0[i].vibActive = 0;
                        padActDirect[0] = 0;
                        padActDirect[1] = 0;
                        scePadSetActDirect(i, 0, padActDirect);
                    }
                }
            }
            if (arg0[i].leftX > 0xD0) {
                arg0[i].held |= 0x8000;
            }
            if (arg0[i].leftX < 0x30) {
                arg0[i].held |= 0x2000;
            }
            if (arg0[i].leftY > 0xD0) {
                arg0[i].held |= 0x1000;
            }
            if (arg0[i].leftY < 0x30) {
                arg0[i].held |= 0x4000;
            }
            arg0[i].repeat = arg0[i].held;
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
            if (i == 1) {
                arg0[i].repeat = arg0[i].repeatFast;
            }
            arg0[i].trigger = arg0[i].held ^ (arg0[i].prevHeld & arg0[i].held);
            arg0[i].prevHeld = arg0[i].held;
            padLastType[i] = padType[i];
            break;
        }
    }
    if (padInputLocked == 1) {
        arg0[0].held = 0;
        arg0[0].prevHeld = 0;
        arg0[0].released = 0;
        arg0[0].repeat = 0;
        arg0[0].trigger = 0;
        arg0[0].leftX = 0x80;
        arg0[0].leftY = 0x80;
        arg0[0].rightX = 0x80;
        arg0[0].rightY = 0x80;
    }
}
