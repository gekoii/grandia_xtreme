#include "common.h"
#include "sys.h"
#include "main/n03.h"
#include "protos/main.h"

typedef struct {
    u8 data[0x70];
} Work;

extern void GsMiscPacketInit(Work *);
extern void MemSet(void *, s32, s32);
extern void sceGsPutDispEnv(void *);
extern void FlushCache(s32);

void SysReinitGraphics(s32 arg0) {
    Work w;

    MemSet(&w, 0, sizeof(w));
    gsDispFrameAddr = 0;
    gsDrawFrameAddr = 0x46000;
    gsZBufAddr = 0x8C000;
    if (sysIsNtsc != 0) {
        sceGsResetGraph(1, 1, 2, 0);
    } else {
        sceGsResetGraph(1, 1, 3, 0);
    }
    if (arg0 != 0) {
        GsVsyncInit();
    }
    GsDmaInit();
    GsVifInit();
    GsMiscPacketInit(&w);
    FlushCache(0);
    SysMfifoStart();
    GsMiscPacketSend(&w);
    SysMfifoStop(0);
    sceGsPutDispEnv(&gsDispEnv);
}
