#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

s32 SndIopGetDriveStat(void *);
s32 GetThreadId(void);
void ReferThreadStatus(s32, struct ThreadParam *);
void FlushCache(s32);

extern s32 movTrpFileTable[];

s32 MovPlayList(s32 type, s32 p1, s32 p2, s32 p3, s32 mem, s32 p5, s32 p6) {
    SndPrepareReq r1;
    SndPlayReq r2;
    struct ThreadParam info;
    s32 a;
    s32 b;
    s32 c;
    s32 *hdr;
    s32 cur;
    s32 i;
    s32 ret;

    if (SndIopGetDriveStat(NULL) & 0x21) {
        return -4;
    }
    movPlayStatus = 1;
    movPadPort = p1;
    movPadSlot = p2;
    movSkipButtons = p3;
    movListType = type;
    mem += (type == 6) ? 0x100000 : 0x80000;
    if (mem & 0x3F) {
        mem = (mem + 0x3F) & 0x7FFFFC0;
    }
    movVoBufData = mem;
    a = mem + 0x578100;
    movVoBufTag = a;
    b = a + 0x10E240;
    movMpegWork = b;
    c = b + 0x13B900;
    movReadBuf = (MovReadBuf *)c;
    movViBufData = (c + 0x18010C) & 0x7FFFFC0;
    if (movListType == 6) {
        hdr = (s32 *)(mem - 0x100000);
    } else {
        hdr = (s32 *)(mem - 0x80000);
    }
    if (SndFsLoadFile(movTrpFileTable[movListType], (s32)hdr) < 0) {
        return -1;
    }
    movTrpStrip = (s32)hdr + hdr[0];
    movTrpClut = (s32)hdr + hdr[1];
    movTrpScroll = (s32)hdr + hdr[2];
    if (movListType == 6) {
        movTrpPicture = (s32)hdr + hdr[3];
        movTrpLabel = (s32)hdr + hdr[4];
    }
    cur = (s32)movReadBuf;
    if (movListType != 6) {
        r1.index = p6 + movListType;
        r1.handle = p5;
        if (SndPrepare(&r1) < 0) {
            return -1;
        }
        r2.handle = p5;
        r2.index = p6 + movListType;
        if (SndPlay(&r2) < 0) {
            return -2;
        }
        movAudioHeld = 0;
    }
    ReferThreadStatus(GetThreadId(), &info);
    movMainPrio = info.currentPriority;
    if (movListType != 6) {
        for (i = 0; i < movPlaylistTable[movListType].count; i++) {
            movFileSize = SndFsFind(movPlaylistTable[movListType].name[i], movFileInfo);
            if (movFileSize < 0) {
                return -1;
            }
            movPlaylistTable[movListType].size[i] = movFileSize;
            movPlaylistTable[movListType].base[i] = (void *)cur;
        }
        movDataStream = SndStrmOpen(movPlaylistTable[movListType].name[0]);
        if (movDataStream < 0) {
            return -2;
        }
        if (SndStrmReadStart(movDataStream, movPlaylistTable[movListType].size[0] >> 11) < 0) {
            SndStrmClose(movDataStream);
            return -7;
        }
    } else {
        for (i = 0; i <= 0; i++) {
            movFileSize = SndFsFind(movPlaylistTable[movListType].name[i], movFileInfo);
            if (movFileSize < 0) {
                return -1;
            }
            movAudioStream = SndStrmOpen(movPlaylistTable[movListType].name[i]);
            if (movAudioStream < 0) {
                return -2;
            }
            movDataStream = SndStrmOpen(movPlaylistTable[movListType].name[i]);
            if (movDataStream < 0) {
                SndStrmClose(movAudioStream);
                return -3;
            }
            movPlaylistTable[movListType].base[i] = (void *)cur;
            movPlaylistTable[movListType].size[i] = movFileSize;
            cur += ((movFileSize + 0x7FF) / 0x800) << 11;
        }
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
    }
    sndMovieStreaming = -1;
    if (movListType == 6) {
        SndIopCmdPush(0x300, movAudioStream, movDataStream, 2);
    } else {
        SndIopCmdPush(0x300, 0, 0, 2);
    }
    SndIopPoll();
    SndBusyWait();
    if (movListType == 6) {
        do {
            SndIopPoll();
            SndBusyWait();
        } while (sndIopStatus->movieState < 2);
    }
    movSkipRequest = 0;
    MovListInit();
    MovListReadLoop(&movVideoDec, &movPlaylistTable[movListType]);
    MovListTerm();
    while (sceGsSyncPath(1, 0) != 0) {
    }
    SndIopPoll();
    SndBusyWait();
    FlushCache(0);
    SndIopCmdPush(0x310, 0, 0, 2);
    if (movListType != 6) {
        do {
            while (sceGsSyncPath(1, 0) != 0) {
            }
            SndIopPoll();
            SndBusyWait();
        } while (SndIopGetDriveStat(NULL) & 0x21);
        ret = SndStop(p5);
        while (sceGsSyncPath(1, 0) != 0) {
        }
        SndIopPoll();
        SndBusyWait();
        if (movAudioHeld != 0) {
            SndSetPause(0);
            movAudioHeld = 0;
        }
        if (ret < 0) {
            return -3;
        }
    } else {
        SndStrmClose(movAudioStream);
        SndStrmClose(movDataStream);
        audioDecReset(&movAudioDec);
        do {
            while (sceGsSyncPath(1, 0) != 0) {
            }
            SndIopPoll();
            SndBusyWait();
        } while (SndIopGetDriveStat(NULL) & 0x21);
        if (movAudioHeld != 0) {
            SndSetPause(0);
            movAudioHeld = 0;
        }
        sndMovieStreaming = 0;
    }
    while (sceGsSyncPath(1, 0) != 0) {
    }
    return movPadButtons;
}
