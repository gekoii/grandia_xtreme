/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "snd.h"





void SndBgmUpdate(void) {
    s32 i;
    s32 k;
    s32 r;

    for (i = 0; i < 2; i++) {
        switch (sndBgmChannels[i].state) {
        case 1:
            r = SndStop(sndBgmChannels[i].handle);
            if (r >= 0 || r == -3) {
                sndBgmChannels[i].state = 2;
            }
            break;
        case 2:
            sndBgmChannels[i].prepare.handle = sndBgmChannels[i].handle;
            sndBgmChannels[i].prepare.index = sndBgmChannels[i].index + i;
            if (SndPrepare(&sndBgmChannels[i].prepare) < 0) {
                sndBgmChannels[i].state = 0;
                sndBgmChannels[i].reqStop = 0;
            } else if (sndBgmChannels[i].fade != 0) {
                sndBgmChannels[i].fadeReq.handle = sndBgmChannels[i].handle;
                sndBgmChannels[i].fadeReq.volume = 0;
                sndBgmChannels[i].fadeReq.speed = 0;
                if (SndSetFade(&sndBgmChannels[i].fadeReq) == 0) {
                    sndBgmChannels[i].curVolume = 0;
                    sndBgmChannels[i].state = 10;
                }
            } else {
                sndBgmChannels[i].state = 10;
            }
            break;
        case 10:
            if (SndGetStatus(sndBgmChannels[i].handle, &sndBgmChannels[i].status) < 0) {
                break;
            }
            if (sndBgmChannels[i].status.stat >= 2) {
                if (sndBgmChannels[i].trackMask != 0) {
                    for (k = 0; k < 8; k++) {
                        if ((sndBgmChannels[i].trackMask >> k) & 1) {
                            SndSetTrackVolume(sndBgmChannels[i].handle, k, 0x3FFF, 0x3FFF);
                        } else {
                            SndSetTrackVolume(sndBgmChannels[i].handle, k, 0, 0);
                        }
                    }
                }
                sndBgmChannels[i].state = 11;
            } else {
                if (sndBgmChannels[i].status.error == 10 || sndBgmChannels[i].status.error == 11) {
                    if (sndBgmChannels[i].status.error == 11) {
                        sndBgmChannels[i].state = 2;
                    }
                }
            }
            break;
        case 11:
            sndBgmChannels[i].play.handle = sndBgmChannels[i].handle;
            sndBgmChannels[i].play.index = sndBgmChannels[i].index + i;
            if (SndPlay(&sndBgmChannels[i].play) >= 0) {
                sndBgmChannels[i].state = 12;
            }
            break;
        case 12:
            r = SndGetStatus(sndBgmChannels[i].handle, &sndBgmChannels[i].status);
            if (r != 0) {
                if (r == -3 || r == -9) {
                    sndBgmChannels[i].state = 0;
                    sndBgmChannels[i].reqStop = 0;
                }
                break;
            }
            if (sndBgmChannels[i].fade != 0) {
                switch (sndBgmChannels[i].fadeState) {
                case 0:
                    sndBgmChannels[i].fadeReq.handle = sndBgmChannels[i].handle;
                    sndBgmChannels[i].fadeReq.volume = sndBgmChannels[i].volume;
                    sndBgmChannels[i].fadeReq.speed = sndBgmChannels[i].fade;
                    if (SndSetFade(&sndBgmChannels[i].fadeReq) == 0) {
                        sndBgmChannels[i].fadeState = 1;
                    }
                    break;
                case 1:
                    if (sndBgmChannels[i].status.nowVol == sndBgmChannels[i].status.disVol) {
                        sndBgmChannels[i].fadeState = 0;
                        sndBgmChannels[i].fade = 0;
                    }
                    sndBgmChannels[i].curVolume = (f32)sndBgmChannels[i].status.nowVol / 10000.0f * 255.0f;
                    break;
                }
            } else if (sndBgmChannels[i].reqStop != 0) {
                sndBgmChannels[i].state = 14;
            }
            if (sndBgmChannels[i].status.stat == 0) {
                /* identical arms: the original keeps the test (16 more insns) */
                if (sndBgmChannels[i].status.error == 11 && sndBgmChannels[i].status.atr == 4) {
                    sndBgmChannels[i].state = 0;
                    sndBgmChannels[i].reqStop = 0;
                } else {
                    sndBgmChannels[i].state = 0;
                    sndBgmChannels[i].reqStop = 0;
                }
            } else if (sndBgmChannels[i].status.atr == 4 && (sndBgmChannels[i].status.error == 10 || sndBgmChannels[i].status.error == 11)) {
                sndBgmChannels[i].state = 0;
                sndBgmChannels[i].reqStop = 0;
            }
            break;
        case 14:
            r = SndStop(sndBgmChannels[i].handle);
            if (r >= 0 || r == -3) {
                sndBgmChannels[i].state = 0;
                sndBgmChannels[i].reqStop = 0;
            }
            break;
        }
    }
}
