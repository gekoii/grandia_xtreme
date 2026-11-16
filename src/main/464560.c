#include "common.h"
#include "protos/main.h"
#include "sdk/libmpeg.h"
#include "main/n11.h"

s16 sceMpegDemuxPssRing(void *, void *, s32, void *, s32);
s32 SndStrmGetReady(s32, s32);
s32 SndIopGetDriveStat(void *);

extern s32 movListReadState;
extern s32 movListReadSectors;

s32 MovListReadLoop(VideoDec *arg0, MovPlaylist *bank) {
    void *buf24;
    void *buf20;
    u8 pad[32];
    s32 ret;
    MovReadBuf *ctx;
    s32 started;
    s32 prev;
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    s32 r;
    s32 trig;
    s32 hit;
    s32 remain;
    s32 remain2;
    s32 stall;
    s32 w;
    s32 t;

    prev = 0;
    ret = 0;
    ctx = bank->base[0];
    i = 0;
    if (bank->count > 0) do {
        for (j = 0; j < bank->repeat[i]; j = n) {
            if (i != 0 || j != 0) {
                MovListTermPart();
                MovListReinit(ctx);
                ctx->count = ctx->put = (s32)bank->base[i] - (s32)bank->base[0];
            }
            stall = 0;
            remain = bank->size[i];
            started = 0;
            movListReadState = 0;
            remain2 = remain;
            movListReadSectors = 0;
            while (remain >= 5 && videoDecGetState(arg0) != 3) {
                if (sceGsSyncPath(1, 0) == 0) {
                    SndIopPoll();
                    for (k = 0; k < 100000; k++) {}
                }
                if (movVblankCount != movPadVblank) {
                    if (scePadRead(movPadPort, movPadSlot, pad) > 0) {
                        movPadButtons = ((pad[2] << 8) | pad[3]) ^ 0xFFFF;
                    } else {
                        movPadButtons = 0;
                    }
                    movPadVblank = movVblankCount;
                }
                trig = (prev ^ movPadButtons) & movPadButtons;
                prev = movPadButtons;
                hit = trig & movSkipButtons;
                if (hit && arg0->mpeg.frameCount >= 11) {
                    movSkipRequest = 1;
                }
                if (movFadeDone != 0 || movOvlEnd != 0) {
                    ret = 1;
                    videoDecAbort(&movVideoDec);
                    break;
                }
                n = readBufBeginPut(ctx, &buf20);
                if (j == 0) {
                    SndIopGetDriveStat(NULL);
                }
                if (remain2 > 0 && n > 0x20000) {
                    switch (movListReadState) {
                    case 0:
                        k = 0x40;
                        if (j == 0) {
                            k = SndStrmGetReady(movDataStream, 0);
                        }
                        if (k < 0) {
                            ret = -0x11;
                        } else if (k >= 0x40) {
                            movListReadSectors = 0x40;
                            if (j == 0) {
                                ret = SndStrmFetchStart(movDataStream, buf20, 0x40);
                                if (ret == 0) {
                                    movListReadState = 1;
                                }
                            } else {
                                movListReadState = 1;
                            }
                        } else if (remain2 < 0x20000 && (k << 11) < n) {
                            if (j == 0) {
                                ret = SndStrmFetchStart(movDataStream, buf20, k);
                                if (ret == 0) {
                                    movListReadState = 1;
                                }
                            } else {
                                movListReadState = 1;
                            }
                        }
                        break;
                    case 1:
                        if (j != 0 || SndStrmFetchPoll(movDataStream) == 0) {
                            k = movListReadSectors << 11;
                            remain2 -= k;
                            readBufEndPut(ctx, k);
                            movListReadState = 0;
                        }
                        ret = 0;
                        break;
                    }
                }
                switchThread();
                n = readBufBeginGet(ctx, &buf24);
                if (n > 0 && movListReadState == 0) {
                    r = sceMpegDemuxPssRing(arg0, buf24, n, ctx, 0x180000);
                    if (r == 0) {
                        if (++stall > 160000) {
                            videoDecAbort(&movVideoDec);
                            for (k = 0; k < 0x80000; k++) {}
                            ret = -3;
                            movPadButtons = movSkipButtons | 0xFE0000;
                            break;
                        }
                        if (movFrameStarve >= 2 && movAudioHeld == 0) {
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
                    readBufEndGet(ctx, r);
                    remain -= r;
                } else if (n == 0) {
                    if (++stall > 100000) {
                        videoDecAbort(&movVideoDec);
                        for (k = 0; k < 0x80000; k++) {}
                        ret = -4;
                        movPadButtons = movSkipButtons | 0xFD0000;
                        break;
                    }
                    if (movFrameStarve >= 2 && movAudioHeld == 0) {
                        if (SndSetPause(1) >= 0) {
                            movAudioHeld = 1;
                        }
                    }
                }
                if (started == 0 && voBufIsFull(&movVoBuf) != 0 && movListReadState == 0 && n > 0x60000) {
                    MovDispStart(1);
                    if (movListType == 6) {
                        audioDecStart(&movAudioDec);
                    }
                    SndBusyWait();
                    started = 1;
                }
                if (remain <= 0x800) {
                    ret = 0;
                    break;
                }
            }
            if (movListType != 6 && j == 0) {
                SndStrmClose(movDataStream);
            }
            if (ret == 0 && i != bank->count - 1 && j == bank->repeat[i] - 1) {
                t = SndStrmOpen(bank->name[i + 1]);
                movDataStream = t;
                if (t < 0) {
                    ret = -1;
                    break;
                }
                if (SndStrmReadStart(t, bank->size[i + 1] >> 11) < 0) {
                    ret = -1;
                    SndStrmClose(movDataStream);
                    break;
                }
            }
            n = j + 1;
            while (videoDecFlush(arg0) == 0) {
                switchThread();
            }
            if (ret != -2) {
                k = 0;
                do {
                    if (videoDecIsFlushed(arg0) != 0 || videoDecGetState(arg0) == 3) {
                        break;
                    }
                    switchThread();
                    for (w = 0; w < 0x1999A; w++) {}
                    k++;
                } while (k < 600);
            }
        }
        if (ret != 0) {
            break;
        }
    } while (++i < bank->count);
    if (ret >= 0 && movOvlEnd != 0 && movFadeDone == 0) {
        do {
            if (sceGsSyncPath(1, 0) == 0) {
                SndIopPoll();
            }
            for (k = 0; k < 0x20000; k++) {}
            switchThread();
            if (movAudioHeld != 0 && movFrameStarve == 0) {
                if (SndSetPause(0) >= 0) {
                    movAudioHeld = 0;
                }
            }
            if (movVblankCount != movPadVblank) {
                if (scePadRead(movPadPort, movPadSlot, pad) > 0) {
                    movPadButtons = ((pad[2] << 8) | pad[3]) ^ 0xFFFF;
                } else {
                    movPadButtons = 0;
                }
                movPadVblank = movVblankCount;
            }
            trig = (prev ^ movPadButtons) & movPadButtons;
            prev = movPadButtons;
            hit = trig & movSkipButtons;
            if (hit) {
                break;
            }
        } while (movFadeDone == 0);
    }
    MovDispEnd();
    return 1;
}
