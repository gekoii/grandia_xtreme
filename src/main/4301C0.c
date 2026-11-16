#include "common.h"
#include "sys.h"
#include "main/n03.h"
#include "protos/main.h"

extern void FlushCache(s32);

void SysClearScreen(void) {
    GsDrawEnvPacketSend(sysClearDrawEnv);
    GsRenderStatePacketSend(sysClearGsState);
    GsGifPacketSend((u16 *)&sysClearPkt);
}

void SysFrameBegin(void) {
    VuMicroLoadVu0();
    GphVu1FrameSetup();
    SysMfifoStart();
    SysClearScreen();
    DbgTextClear();
    GphResetSprLight();
    GphClearRenderStats();
}

void SysFrameEnd(void) {
    sysFrameCount += 1;
    GphFbSwapDrawZBuf();
    GphFbFlipCopy();
    SysMfifoStop(0);
}

s32 SysGetFrameCount(void) {
    return sysFrameCount;
}

void SysClearBuffers(u32 arg0) {
    SysSetClearColor(arg0);
    FlushCache(0);
    SysMfifoStart();
    SysClearScreen();
    SysMfifoStop(0);
    SysWaitFrame(0);
    FlushCache(0);
    SysMfifoStart();
    SysClearScreen();
    SysMfifoStop(0);
    SysWaitFrame(0);
    FlushCache(0);
}
