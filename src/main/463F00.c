#include "common.h"
#include "protos/main.h"
#include "sdk/libmpeg.h"
#include "main/n11.h"

s32 SndIopGetDriveStat(void *);
s32 SndStrmGetReady(s32, s32);
s16 sceMpegDemuxPssRing(VideoDec *, void *, s32, MovReadBuf *, s32);

extern s32 movReadState;
extern s32 movReadSectors;

s32 MovReadLoop(VideoDec *arg0, MovReadBuf *arg1, s32 arg2, s32 arg3) {
    u8 buf[0x20];
    void *rp;
    void *wp;
    s32 started;
    s32 prev;
    s32 remain;
    s32 left;
    s32 ret;
    s32 stall;
    s32 n;
    s32 m;
    s32 i;
    s32 j;
    s32 t;

    remain = arg3;
    left = remain;
    started = 0;
    ret = 0;
    prev = 0;
    stall = 0;
    movReadState = 0;
    movReadSectors = 0;
    while (remain >= 5) {
        if (videoDecGetState(arg0) == 3) {
            break;
        }
        if (sceGsSyncPath(1, 0) == 0) {
            SndIopPoll();
            for (i = 0; i < 100000; i++) {
            }
        }
        if (movVblankCount != movPadVblank && movSkipRequest == 0) {
            if (scePadRead(movPadPort, movPadSlot, buf) > 0) {
                movPadButtons = ((buf[2] << 8) | buf[3]) ^ 0xFFFF;
            } else {
                movPadButtons = 0;
            }
            movPadVblank = movVblankCount;
        }
        t = movPadButtons;
        j = prev ^ t;
        prev = t;
        if ((j & t & movSkipButtons) && arg0->mpeg.frameCount >= 11) {
            movSkipRequest = 1;
        }
        if (movFadeDone != 0) {
            ret = 1;
            videoDecAbort(&movVideoDec);
            break;
        }
        n = readBufBeginPut(arg1, &rp);
        SndIopGetDriveStat(NULL);
        if (left > 0 && n > 0x20000) {
            switch (movReadState) {
            case 0:
                i = SndStrmGetReady(movDataStream, 0);
                if (i < 0) {
                    ret = -17;
                } else if (i >= 0x40) {
                    movReadSectors = 0x40;
                    ret = SndStrmFetchStart(movDataStream, rp, 0x40);
                    if (ret == 0) {
                        movReadState = 1;
                    }
                } else if (left < 0x20000 && (i << 11) < n) {
                    ret = SndStrmFetchStart(movDataStream, rp, i);
                    if (ret == 0) {
                        movReadState = 1;
                    }
                }
                break;
            case 1:
                if (SndStrmFetchPoll(movDataStream) == 0) {
                    i = movReadSectors << 11;
                    left -= i;
                    readBufEndPut(arg1, i);
                    movReadState = 0;
                }
                break;
            }
        }
        switchThread();
        m = readBufBeginGet(arg1, &wp);
        if (m > 0 && movReadState == 0) {
            i = sceMpegDemuxPssRing(arg0, wp, m, arg1, arg1->size);
            if (i == 0) {
                stall++;
                if (stall > 40000) {
                    videoDecAbort(&movVideoDec);
                    i = 0x80000;
                    while (--i != 0) {
                    }
                    ret = -3;
                    movPadButtons = movSkipButtons | 0xFE0000;
                    break;
                }
                if (movFrameStarve >= 2 && movAudioHeld == 0 && sndIopStatus->strm[2].secNum < 16) {
                    if (SndSetPause(1) >= 0) {
                        movAudioHeld = 1;
                    }
                }
            } else {
                if (movAudioHeld != 0 && movFrameStarve == 0) {
                    if (SndSetPause(0) >= 0) {
                        movAudioHeld = 0;
                    }
                }
                stall = 0;
            }
            readBufEndGet(arg1, i);
            remain -= i;
        } else if (m == 0) {
            stall++;
            if (stall > 100000) {
                videoDecAbort(&movVideoDec);
                i = 0x80000;
                while (--i != 0) {
                }
                ret = -4;
                movPadButtons = movSkipButtons | 0xFD0000;
                break;
            }
            if (movFrameStarve >= 2 && movAudioHeld == 0 && sndIopStatus->strm[2].secNum < 16) {
                if (SndSetPause(1) >= 0) {
                    movAudioHeld = 1;
                }
            }
        }
        if (started == 0 && voBufIsFull(&movVoBuf) != 0 && movReadState == 0 &&
            sndIopStatus->movieState >= 3 && m > 0x60000) {
            started = 1;
            MovDispStart(1);
            audioDecStart(&movAudioDec);
            SndBusyWait();
        }
        if (remain < 0x801) {
            ret = 0;
            break;
        }
    }
    while (videoDecFlush(arg0) == 0) {
        switchThread();
    }
    if (ret != -2) {
        i = 0;
        do {
            if (videoDecIsFlushed(arg0) != 0) {
                break;
            }
            if (videoDecGetState(arg0) == 3) {
                break;
            }
            switchThread();
            j = 0x1999A;
            while (--j != 0) {
            }
        } while (++i < 600);
    }
    MovDispEnd();
    if (movReadState != 0) {
        while (SndStrmFetchPoll(movDataStream) == 0) {
            while (sceGsSyncPath(1, 0) != 0) {
            }
            SndIopPoll();
            SndBusyWait();
        }
    }
    return 1;
}
