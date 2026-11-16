#include "common.h"
#include "sys.h"
#include "gph.h"
#include "main/n02.h"
#include "chr.h"

void MemSet(void *, s32, s32);
extern f32 dbgMotSpeedDefault;

void DbgViewInit(void) {
    s32 i;

    MemSet(&dbgMemView, 0, 0x1C);
    dbgMemView.base = 0x770000;
    gphFbEffParam[0] = 0;
    gphFbEffParam[1] = 0x40;
    gphFbEffParam[2] = 0x20;
    gphFbEffParam[6] = gphFbEffParam[3] = 1;
    gphFbDivParam.alpha = 0x80;
    gphFbEffParam[4] = 0;
    gphFbEffParam[5] = 0;
    gphFbDivParam.blur = 0;
    gphFbEffParam[7] = 0x30;
    gphFbDivParam.bokeh = 0;
    gphFbDivParam.x = 0x140;
    gphFbDivParam.y = 0xE0;
    gphFbDivParam.blow = 0;
    gphFbDivParam.u = 0;
    gphFbDivParam.v = 0;
    gphFbDivParam.aMode = 0;
    gphFbDivParam.w = 0x280;
    gphFbDivParam.theta = 0;
    gphFbDivParam.h = 0x1C0;
    for (i = 0; i < 4; i++) {
        gphFbDivParam.r[i] = 0;
        gphFbDivParam.g[i] = 0;
        gphFbDivParam.b[i] = 0;
        gphFbDivParam.a[i] = 0x80;
    }
    sysState.unk62 = 1;
    sysState.unk50 = NULL;
    sysState.screenCaptureReq = 0;
    dbgClearR = 0x40;
    dbgClearG = 0x40;
    dbgClearB = 0x40;
    dbgObjNo = 0;
    dbgObjList = NULL;
    for (i = 0; i < 32; i++) {
        dbgMotObjParams[i].motionNo = -1;
        dbgMotObjParams[i].speed = dbgMotSpeedDefault;
        dbgMotObjParams[i].mode = 1;
        dbgMotObjParams[i].unk4 = 1;
    }
    dbgMotObjParams[0].flag = 0;
    dbgMotObjParams[1].flag = 0;
    dbgMotObjParams[2].flag = 0;
}
