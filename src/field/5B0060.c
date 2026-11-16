#include "common.h"
#include "sys.h"
#include "gph.h"
#include "grxsys.h"
#include "save.h"
#include "party.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n16.h"
#include "field/n27.h"
#include "protos/field.h"

/* evtScnrSysVarIds: the header's u16 fields are stored with sw here */
typedef struct {
    u8 pad0[4];
    s32 resultVar;
    s32 restartFlag;
    s32 autoFlag;
    s32 noSkipFlag;
} SysVarIds;

/* EvtMsgLayoutSet record: {slot, x0, y0, x1, y1} */
typedef struct {
    s16 slot, x0, y0, x1, y1;
} MsgLayout;

void MemSet(void *, s32, s32);
void EvtMsgLayoutSet(MsgLayout *, s32);
extern f32 fldScaleUnused;
extern TxtFontImage fldMsgFontImages[3];

void FldInit(void) {
    void **q;
    MsgLayout r;
    TxtFontImage *o;
    s32 buf;
    s32 k;

    MemHeapSelect(sysState.heap20);
    MemHeapReset(sysState.heap20);
    MemSet(ptyBattleParams, 0, 4);
    MemSet(ptyPartyUnits, 0, 4);
    MemSet(&btlHandoff, 0, 0x140);
    MemSet(ptyCharData, 0, 0x420);
    MemSet(&sysSaveData, 0, 0x3300);
    {
        s32 p1 = MemAlloc32(0x4000);
        MemSet((void *)p1, 0, 0x4000);
        EvtMsgTileGridInit((s16 *)EvtScnrInit(p1));
    }
    {
        s32 p2 = MemAlloc32(0x800);
        MemSet((void *)p2, 0, 0x800);
        EvtScnrSetVarBuf(p2);
    }
    q = (void **)&evtMapFlagTable;
    {
        void *t = (void *)MemAlloc32(0x90);
        *q = t;
        MemSet(t, 0, 0x90);
    }
    evtScnrWork->vars = (u8 *)&sysSaveData.mainFlag;
    ((SysVarIds *)evtScnrSysVarIds)->resultVar = 4;
    ((SysVarIds *)evtScnrSysVarIds)->noSkipFlag = 0xA33;
    ((SysVarIds *)evtScnrSysVarIds)->autoFlag = 0xA52;
    ((SysVarIds *)evtScnrSysVarIds)->restartFlag = 0xA4C;
    ((EvtScnrGameRefs *)EvtScnrGetGameRefs())->money = (s32 *)ptyPartyData;
    EvtScnrSetScriptTable(0);
    EvtScnrSetDebug(1);
    r.slot = 0; r.x0 = 2; r.y0 = 0x15; r.x1 = 0x26; r.y1 = 0x1B;
    EvtMsgLayoutSet(&r, 0);
    r.slot = 1; r.x0 = 3; r.y0 = 0xC; r.x1 = 0x24; r.y1 = 0xF;
    EvtMsgLayoutSet(&r, 1);
    r.slot = 1; r.x0 = 3; r.y0 = 3; r.x1 = 0x26; r.y1 = 0xA;
    EvtMsgLayoutSet(&r, 2);
    r.slot = 1; r.x0 = 0x10; r.y0 = 0x15; r.x1 = 0x24; r.y1 = 0x18;
    EvtMsgLayoutSet(&r, 3);
    r.slot = 1; r.x0 = 0x13; r.y0 = 1; r.x1 = 0x14; r.y1 = 2;
    EvtMsgLayoutSet(&r, 4);
    r.slot = 1; r.x0 = 0x13; r.y0 = 2; r.x1 = 0x14; r.y1 = 1;
    EvtMsgLayoutSet(&r, 5);
    r.slot = 1; r.x0 = 0x26; r.y0 = 0xA; r.x1 = 0x10; r.y1 = 2;
    EvtMsgLayoutSet(&r, 6);
    for (k = 0; k < 3; k++) {
        o = &fldMsgFontImages[k];
        buf = TxtFontImageCalcSize(o, 0x10, 0x10, 0x200, 0x100);
        TxtFontImageSetup(o, 0x10, 0x10, 0x200, 0x100, sysGrxsysPac->fontTex, 0, MemAlloc64(buf));
        TxtFontImageClear(o);
        o->glyphMetricsSrc = sysGrxsysPac->glyphTable;
    }
    AmapWorkClear();
    FldNewGameReset();
    EvtStateResetAll();
    MapEntranceReset();
    ItemPartyInitNew();
    MapLoadStateInit();
    MemHeapSelect(sysState.dataHeap);
    FldMapCtlInit();
    FldSndReset();
    GphFadeClear(&fldFades[0]);
    GphFadeClear(&fldFades[1]);
    GphFadeClear(&fldFades[2]);
    GphScreenFxClear(&fldScreenFx[0]);
    GphScreenFxClear(&fldScreenFx[1]);
    if (sysState.demoBoot == 0) {
        MapJumpRequest(sysIni.mapNumber, sysIni.mapWarpNo);
    }
    fldState.talkRotSpeed = 1.0f;
    fldState.systemSpeed = 2;
    fldState.unk10 = 1;
    fldState.encountRange = 1.0f;
    camCtl.aiType = 5;
    fldState.kamaeSpeed = 1.0f;
    fldState.runSpeed = 1.0f;
    sysState.pauseMode = 0;
    sysState.dbgMenuFlags = 0;
    fldScaleUnused = 1.0f;
    sysState.dbgDigit = 1;
    GphFadeStart(&fldFades[0], 0, 0.0f, 0.0f, 0.0f, 0.0f, 30.0f);
    GphFadeStart(&fldFades[1], 0, 0.0f, 0.0f, 0.0f, 0.0f, 30.0f);
    sysSaveData.optMiniMapOff = 1;
    sysSaveData.mainFlag = sysIni.mainFlagNo;
    FldSceneSetup(sysIni.mainFlagNo);
    McInit();
    if (SysDemoGetMode() != 0) {
        fldState.mode = 4;
    }
}
