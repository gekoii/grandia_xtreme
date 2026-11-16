#include "common.h"
#include "protos/main.h"
#include "grxsys.h"
#include "save.h"
#include "sys.h"


void SysDemoStart(s32 arg0) {
    SysDemo *p;
    f32 z;

    SysDemoReset();
    p = &sysDemo;
    sysSoftResetLock = 1;
    p->ready = 0;
    p->unk44 = 0;
    p->mode = arg0;
    p->promptAlpha = 0.0f;
    p->captionAlpha0 = 0.0f;
    p->captionAlpha1 = 0.0f;
    if (arg0 == 1) {
        sysState.movieNo = 0;
        sysState.progChangeReq = 1;
        sysState.nextMapNo = sysDemoMapIds[VuRandInt(6)];
    }
    z = 0.0f;
    GphFadeSetColor(&p->fade, z, z, z, z);
    GphFadeSetColor(&p->fade2, z, z, z, z);
    GsTexLoadPacket(p->texPacket, sysGrxsysPac->tex150, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
    sysSaveData.optVibration = 1;
    sysSaveData.optCamReverse = 0;
    sysSaveData.optSoundMode = 0;
    sysSaveData.optIpStop = 0;
    sysSaveData.optMiniMapOff = 0;
    sysSaveData.optMiniMap2A = 0;
    sysSaveData.optMiniMap2B = 0;
    PadSetVibEnabled(0);
    SndSetOutputMode(sysSaveData.optSoundMode);
}
