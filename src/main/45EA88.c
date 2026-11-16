#include "common.h"
#include "main/n09.h"
#include "main/n07.h"
#include "protos/main.h"

void sceGsPutDispEnv(GsDispEnv *);

s32 GsVsyncFlip(s32 arg0) {
    u32 t;
    u64 u;

    sceGsSyncPath(0, 0);
    gsFlipWaitCount = arg0 + 1;
    while (gsFlipWaitCount != 0) {
    }
    u = gsDispFrameAddr;
    t = gsDrawFrameAddr;
    gsDispEnv.dispfb = (gsDispEnv.dispfb & ~0x1FF) | (t >> 11);
    gsDrawFrameAddr = u;
    gsDispFrameAddr = t;
    sceGsPutDispEnv(&gsDispEnv);
    return (*(volatile u64 *)0x12001000 >> 13) & 1;
}
