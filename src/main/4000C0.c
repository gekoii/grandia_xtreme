#include "common.h"
#include "protos/main.h"
#include "pad.h"
#include "save.h"
#include "sys.h"
#include "gph.h"
#include "main/n01.h"

typedef struct {
    u16 unk0;
    u16 unk2;
    s32 unk4;
} __attribute__((aligned(8))) Flags;

extern void MemSet(void *, s32, s32);
extern void FlushCache(s32);
extern u8 mcLastPort;
extern u8 mcLastSlot;

void SysMain(s32 argc, char **argv) {
    s32 i;
    s32 ok;
    u32 one;
    s32 r;
    s32 n;

    __main();
    SysBoot(1, argv[0]);
    SysInitEngine(0);
    MemSet(&sysState, 0, 0x1400);
    MemSet(&sysSaveData, 0, 0x3300);
    mcLastSlot = 0;
    mcLastPort = 0;
    for (i = 0; i < 8; i++) {
        sysState.active[i] = 0xFF;
    }
    sysState.unk62 = 1;
    sysState.activeCount = 1;
    one = 1;
    MemInit(7, 0x760000, 0x1FF0000);
    sysState.battleHeap = MemHeapCreateAt(0x200000, 0x100000);
    sysState.faceHeap = MemHeapCreateAt(0x300000, 0x100000);
    sysState.heap30 = MemHeapCreate(0x620000);
    sysState.heap38 = MemHeapCreate(0x420000);
    sysState.heap20 = MemHeapCreate(0x40000);
    sysState.battleMagicHeap = MemHeapCreate(0x120000);
    sysState.dataHeap = MemHeapCreate(0xCE0000);
    SysDataInit();
    SysTexturesBind();
    SysIniLoad(&sysState);
    sysState.demoBoot = 0;
    if (sysState.program == one) {
        sysState.demoBoot = one;
        sysState.program = 2;
    }
    SysPartyPacLoad();
    if (SysIsDebugMode() == 0) {
        srand(GsGetTimerCount());
    }
    MemHeapSelect(sysState.dataHeap);
    DbgTextSetCharSize(12, 20);
    SysSetClearColor(0x80000000);
    GphScreenFadeSetColor(0.0f, 0.0f, 0.0f, 0.0f);
    DbgViewInit();
    SysDemoReset();
    n = sysState.program;
    sysState.battleBoot = 0;
    ok = 0;
    if ((u8)(n - 2) <= one) {
        ok = 1;
    }
    if (n == 3) {
        sysState.battleBoot = one;
    }
    sysState.active[0] = n;
    sysState.requested[0] = n;
    sysState.active[1] = 3;
    sysState.requested[1] = 3;
    SysProgLoad(sysState.active[0]);
    if (sysState.active[0] != 3 && ok) {
        SysProgLoad(sysState.active[1]);
    }
    sysState.program = sysState.active[0];
    SysProgCall(1, 0, 0);
    SysProgCall(3, 0, 0);
    if (sysState.active[0] != 3 && ok) {
        sysState.program = sysState.active[1];
        SysProgCall(1, 0, 0);
        SysProgCall(3, 0, 0);
        sysState.program = sysState.active[0];
    }
    if (sysState.program == 3) {
        DbgBattlePreSetup();
        DbgBattleSetup();
        SysMfifoStart();
        SysProgCall(2, 0, 0);
        SysMfifoStop(0);
    }
    if (sysState.demoBoot != 0) {
        SysDemoStart(0x63);
    }
    FlushCache(0);
    for (;;) {
        sysState.frame++;
        sysState.vblankCount = GsGetVblankCount();
        PadUpdate(padPort0);
        if ((padPort0[0].held & 4) && (padPort0[0].held & 1) && (padPort0[0].held & 0x90A) == 0x90A) {
            r = SndSeBankLoadGetState();
            if (r == -1 && FileGetReadState() == r && btlHandoff.softResetLock == 0) {
                btlHandoff.softResetReq = 1;
                GphScreenFadeSetColor(0.0f, 0.0f, 0.0f, 128.0f);
            }
        }
        if (sysState.screenCaptureReq != 0) {
            GphFbStoreImage(0, 0, 0x280, 0x1C0, sysState.screenCaptureAddr);
            sysState.screenCaptureReq = 0;
        }
        if (SysIsDebugMode() != 0) {
            if (padPort0[1].trigger & 0x800) {
                if (sysState.pauseMode != 0) {
                    sysState.pauseMode = 0;
                } else {
                    sysState.pauseMode = 1;
                }
            } else if (sysState.pauseMode != 0) {
                sysState.pauseMode = 2;
                if (padPort0[1].trigger & 0x80) {
                    sysState.pauseMode = 1;
                }
            }
        }
        if (SysIsDebugMode() != 0 && (padPort0[1].held & 1) && (padPort0[1].trigger & 4)) {
            DbgHotkeyStub();
        }
        sysPerfTimes[0] = GsGetFrameTimePercent();
        SysFrameBegin();
        sysPerfTimes[1] = GsGetFrameTimePercent();
        GsPacketBufSwap(gsMainPacketBuf);
        GsSetPacketBuf(gsMainPacketBuf);
        GphTexTableSelect(gphTexTableBuf);
        SysProgDrawAll();
        GphTexTableSelect(gphTexTableBuf);
        GphShatterDraw();
        SysDemoDraw();
        sysPerfTimes[2] = GsGetFrameTimePercent();
        GphFbEffectPass();
        sysPerfTimes[3] = GsGetFrameTimePercent();
        SysProgApplyRequests();
        SysProgUpdateAll();
        SysDemoUpdate();
        GphShatterUpdate();
        sysPerfTimes[4] = GsGetFrameTimePercent();
        if (SysIsDebugMode() != 0) {
            DbgMenuUpdate();
        }
        SysProgBuildDrawAll();
        sysPerfTimes[5] = GsGetFrameTimePercent();
        GphViewSelect(7, 0.0f);
        GphWipeUpdate();
        GphScreenFadeUpdate();
        GphScreenFadeDraw();
        sysPerfTimes[6] = GsGetFrameTimePercent();
        if (SysIsDebugMode() != 0) {
            DbgLoadMeterDraw(0);
            DbgTextDraw();
        }
        sysPerfTimes[7] = GsGetFrameTimePercent();
        SysFrameEnd();
        sysPerfTimes[8] = GsGetFrameTimePercent();
        SysFrameSync();
        sysState.loopFrameCount++;
        if (sysState.progChangeReq != 0) {
            SndSeSuspend();
            SndSeStopAll();
            SndBgmFadeOut(0, 0);
            SndBgmFadeOut(1, 0);
            if (SndBgmGetState(0) == 0 && SndBgmGetState(1) == 0) {
                r = FileGetReadState();
                if (r == -1 && SndSeBankLoadGetState() == r && sysState.unk11 == 0) {
                    SysMoviePlay(sysState.movieNo, 0x20);
                    sysState.progChangeReq = 0;
                    sysState.unkE = 1;
                    GphTexTableInit(0x100, (s32)gphTexTableBuf);
                    GphTexTableSelect(gphTexTableBuf);
                    SysTexturesBind();
                    SysProgCall(2, 0, 0);
                    SysFrameSync();
                }
            }
        }
        if (btlHandoff.unk0 != 0 || btlHandoff.softResetReq != 0) {
        n = sysState.program;
        if (n == 3) {
            sysState.program = 2;
            n = 2;
        }
        sysState.active[0] = n;
        sysState.requested[0] = n;
        sysState.active[1] = 3;
        sysState.requested[1] = 3;
        GphShatterStop();
        SndSeStopAll();
        SndBgmFadeOut(0, 0);
        SndBgmFadeOut(1, 0);
        PadVibrate(padPort0, 0, 0, 0);
        do {
            PadUpdate(padPort0);
            SysFrameBegin();
            SysFrameEnd();
            SysFrameSync();
        } while (SndBgmGetState(0) != 0 || SndBgmGetState(1) != 0 || (r = SndSeBankLoadGetState()) != -1 || FileGetReadState() != r);
        GphScreenFadeSetColor(0.0f, 0.0f, 0.0f, 0.0f);
        SysExitHook();
        SysClearBuffers(0x80000000);
        MemHeapReset(sysState.heap20);
        MemHeapReset(sysState.dataHeap);
        GphTexTableInit(0x100, (s32)gphTexTableBuf);
        GphTexTableSelect(gphTexTableBuf);
        SysTexturesBind();
        if (sysState.program != 3) {
            SysProgCall(1, 0, 0);
        }
        SysProgCall(3, 0, 0);
        if (sysState.program == 3) {
            DbgBattlePreSetup();
            DbgBattleSetup();
            SysMfifoStart();
            SysProgCall(2, 0, 0);
            SysMfifoStop(0);
        } else if (sysState.demoBoot != 0) {
            MemHeapReset(sysState.dataHeap);
            SysDemoStart(1);
            sysState.unkD = 1;
            sysState.unkE = 1;
            sysState.progChangeReq = 0;
            SysProgCall(2, 0, 0);
        }
        SysFrameSync();
        SysSetClearColor(0x80000000 | (sysIni.backColor[2] << 16) | (sysIni.backColor[1] << 8) | sysIni.backColor[0]);
        DbgSetClearColor(sysIni.backColor[0], sysIni.backColor[1], sysIni.backColor[2]);
        DbgLightAngleSet(0, sysIni.light0Angle);
        DbgLightAngleSet(1, sysIni.light1Angle);
        btlHandoff.unk0 = 0;
        btlHandoff.softResetReq = 0;
        }
    }
}
