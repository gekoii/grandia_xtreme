#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n03.h"

extern void GsMiscPacketInit(void *);
extern void *MemSet(void *, s32, s32);
extern void sceGsResetPath(void);
extern void sceGsPutDispEnv(void *);
extern void FlushCache(s32);
extern void sceGsDefDispEnv(void *, s32, s32, s32, s32, s32);

void SysInitEngine(s32 arg0) {
    u8 buf[0x70];

    MemSet(buf, 0, 0x70);
    sceGsResetPath();
    sceDmaReset(1);
    if (arg0 == 0) {
        if (sysIsNtsc != 0) {
            sceGsResetGraph(0, 1, 2, 0);
        } else {
            sceGsResetGraph(0, 1, 3, 0);
        }
        GsVsyncInit();
    }
    GsDmaInit();
    GsVifInit();
    GsMiscPacketInit(buf);
    FlushCache(0);
    SysMfifoStart();
    GsMiscPacketSend(buf);
    SysMfifoStop(0);
    gsDispFrameAddr = 0;
    gsDrawFrameAddr = 0x46000;
    gsZBufAddr = 0x8C000;
    VuMicroLoadVu0();
    FileInit();
    GphViewInitAll();
    SysMfifoStart();
    GphViewSelect(0, 0.0f);
    SysMfifoStop(0);
    GphFbEffectInit();
    GphBatchInit();
    GphScreenFadeInit();
    CamInit(0, 0);
    SysParamInit(0, 0);
    GphLightModuleInit();
    ChrModelPoolReset();
    DbgTextSetOption(0, 0);
    ChrModelAttrInit();
    FileChunkInit();
    PtclInit();
    GphWipeInit();
    GphDrawListModuleInit();
    GphModelModuleInit();
    TxtFontClutInit();
    GphPrimInit();
    SndSeInit();
    ChrPartUtilInit();
    VuInit();
    FilePacInit();
    GphAnimSetClear();
    GphProjTexClear();
    GphShadowInit();
    GphEnvMapInit();
    GphModelDrawSimpleInit();
    GphScreenFxInit();
    PtclEmitterSysInit();
    GphRippleModuleInit();
    GphDeformInit();
    McDriverInit();
    if (arg0 == 0) {
        SysClearBuffers(0x80000000);
        if (sysIsNtsc != 0) {
            sceGsDefDispEnv(&gsDispEnv, 0, 0x280, 0x1C0, 0, 0);
        } else {
            sceGsDefDispEnv(&gsDispEnv, 0, 0x280, 0x1C0, 0, 0x20);
        }
        sceGsPutDispEnv(&gsDispEnv);
    }
}
