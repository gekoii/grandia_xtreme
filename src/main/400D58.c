#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "sys.h"



typedef struct {
    u8 pad0[0x1C];
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
} Timer;



void SysFrameSync(void) {
    f32 t;
    s32 n;
    s32 m;
    s32 i;

    SysProgEndFrameAll();
    SndSeBankLoadUpdate();
    SndBgmUpdate();
    FileReadUpdate(0);
    SndSeUpdate();
    SndIopPoll();
    sysPerfTimes[9] = GsGetFrameTimePercent();
    if (sysState.frameSkip != 0) {
        n = GsGetVblanksSinceFlip();
        m = sysState.frameMode;
        if (n < m) {
            n = m;
        }
        if (n != 0) {
            padPort0[0].trigger = 0;
            padPort0[1].trigger = 0;
            SysMfifoStart();
            for (i = 0; i < n; i++) {
                PadUpdateRepeat(padPort0);
                SysProgApplyRequests();
                SysProgUpdateAll();
            }
            SysProgBuildDrawAll();
            SysMfifoStop(0);
            SysProgEndFrameAll();
        }
    }
    t = GsGetFrameTimePercent();
    sysPerfTimes[10] = t;
    sysState.frameTime = t;
    sysState.frameTimeDelta = sysPerfTimes[8] - sysPerfTimes[7];
    GsVsyncFlip(sysState.frameMode);
}
