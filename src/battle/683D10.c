#include "common.h"
#include "battle/misc.h"
#include "battle/n28.h"
#include "grxsys.h"
#include "sys.h"
#include "protos/battle.h"


void BtlDropInit(void) {
    BtlDropObj *p;
    s32 i;

    btlCommonModelPack = sysGrxsysPac->modelPackB0;
    btlHandoffDropCount = 0;
    btlElementExp[3] = 0;
    btlElementExp[2] = 0;
    btlGoldTotal = 0;
    btlGoldDropCount = 0;
    btlExpTotal = 0;
    btlElementExp[1] = 0;
    btlElementExp[0] = 0;
    p = btlDropObjs;
    for (i = 0; i < 12; i++) {
        p->kind = 0;
        p->modelBuf = 0;
        p->drawList = 0;
        p->coins = NULL;
        p++;
    }
    EnvShadowAddCallback(BtlDropShadowPrepare, BtlDropShadowDraw, NULL);
}
