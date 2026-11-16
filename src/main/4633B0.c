#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

s32 SndIopGetDriveStat(s32);
s32 GetThreadId(void);
s32 ReferThreadStatus(s32, struct ThreadParam *);
void FlushCache(s32);
extern s32 movStrFile;

s32 MovPlay(char *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    struct ThreadParam tp;
    s32 a;
    s32 b;
    s32 c;
    s32 r;

    if (SndIopGetDriveStat(0) & 0x21) {
        return -4;
    }
    movPadPort = arg1;
    movPadSlot = arg2;
    movSkipButtons = arg3;
    movPlayStatus = 1;
    movFileSize = SndFsFind(arg0, movFileInfo);
    if (movFileSize < 0) {
        return -1;
    }
    movAudioStream = SndStrmOpen(arg0);
    if (movAudioStream != 0) {
        return -2;
    }
    movDataStream = SndStrmOpen(arg0);
    if (movDataStream != 1) {
        SndStrmClose(movAudioStream);
        return -3;
    }
    ReferThreadStatus(GetThreadId(), &tp);
    movMainPrio = tp.currentPriority;
    if (arg4 & 0x3F) {
        arg4 = (arg4 + 0x3F) & 0x07FFFFC0;
    }
    movVoBufData = arg4;
    a = arg4 + 0x578100;
    movVoBufTag = a;
    b = a + 0x10E240;
    movMpegWork = b;
    c = (b + 0x13B900) & 0xFFFFFFC0;
    movReadBuf = (MovReadBuf *)c;
    movViBufData = (c + 0x18010C) & 0x07FFFFC0;
    if (movPlayStatus != 0) {
        SndIopPoll();
        SndBusyWait();
        if (sndIopStatus->strm[2].stat != 0) {
            movPlayStatus = -0x40;
        }
        if (sndIopStatus->strm[2].secNum > 0) {
            movPlayStatus = -0x41;
        }
        if (movPlayStatus < 0) {
            SndStrmClose(movAudioStream);
            SndStrmClose(movDataStream);
            return movPlayStatus;
        }
        while (SndIopCmdPush(0x208, 0, 0, 0) == 0) {
            SndIopPoll();
            SndBusyWait();
        }
    }
    if (SndStrmReadStart(movAudioStream, movFileSize >> 11) < 0) {
        SndStrmClose(movAudioStream);
        SndStrmClose(movDataStream);
        return -7;
    }
    sndMovieStreaming = -1;
    SndIopCmdPush(0x300, movAudioStream, movDataStream, 2);
    SndIopPoll();
    SndBusyWait();
    do {
        SndIopPoll();
        SndBusyWait();
    } while (sndIopStatus->movieState < 2);
    r = sndIopStatus->strm[movDataStream].unk1C;
    movSkipRequest = 0;
    movAudioHeld = 0;
    MovInit();
    MovReadLoop(&movVideoDec, movReadBuf, movStrFile, r);
    MovTerm();
    while (sceGsSyncPath(1, 0) != 0) {
    }
    SndIopPoll();
    SndBusyWait();
    FlushCache(0);
    SndIopCmdPush(0x310, 0, 0, 2);
    SndStrmClose(movAudioStream);
    SndStrmClose(movDataStream);
    audioDecReset(&movAudioDec);
    do {
        while (sceGsSyncPath(1, 0) != 0) {
        }
        SndIopPoll();
        SndBusyWait();
    } while (SndIopGetDriveStat(0) & 0x21);
    if (movAudioHeld != 0) {
        SndSetPause(0);
        movAudioHeld = 0;
    }
    sndMovieStreaming = 0;
    while (sceGsSyncPath(1, 0) != 0) {
    }
    return movPadButtons;
}
