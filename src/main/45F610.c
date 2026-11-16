#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

typedef struct {
    u8 pad[0xA2];
    u8 a2;
    u8 a3;
    u8 a4;
    u8 a5;
    u8 pad6[4];
    u8 aa;
    u8 ab;
    u8 ac;
    u8 ad;
} Hdr;

s32 SndFsReadDir(s32, s32, s32);
extern s32 sndFsRootDirSize;
extern s32 sndFsRootDirLsn;
extern s32 sndFsDirBuf;
extern Hdr sndFsSectorBuf;

s32 SndFsMount(s32 arg0, s32 arg1, char *name) {
    s32 ret;

    ret = SndIopCmdPush(0x100, 0, 0, 0);
    SndIopPoll();
    SndBusyWait();
    while (SndFsReadRawSectors(&sndFsSectorBuf, 0x10, 1) < 0) {
    }
    sndFsDirBuf = arg1;
    sndFsRootDirLsn = (sndFsSectorBuf.a2 << 24) + (sndFsSectorBuf.a3 << 16) + (sndFsSectorBuf.a4 << 8) + sndFsSectorBuf.a5;
    sndFsRootDirSize = (sndFsSectorBuf.aa << 24) + (sndFsSectorBuf.ab << 16) + (sndFsSectorBuf.ac << 8) + sndFsSectorBuf.ad;
    while (SndFsReadDir(sndFsRootDirLsn, sndFsRootDirSize, arg0) < 0) {
        SndIopPoll();
        SndBusyWait();
    }
    sndFsRootDir = (SndFsDirEntry *)arg0;
    sndFsCurDir = (SndFsDirEntry *)arg0;
    return ret;
}
