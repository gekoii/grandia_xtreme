#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/state.h"
#include "field/n20.h"


s32 MapMiniIsHidden(s32 arg0) {
    s32 st;
    ChrObj *c;

    st = 0;
    if (MapMiniIndexGet(EvtGetMapNo()) == -1) {
        return 1;
    }
    if (arg0 == 1) {
        if (sysSaveData.optMiniMapOff != 0) { st = arg0; }
    }
    if (MapLoadGetState() != -1) {
        st = 2;
    }
    if (mapMiniWork->model == 0) {
        st = 3;
    }
    if (fldState.mode != FLD_MODE_PLAY) {
        st = 6;
    }
    if (MiniGetState() != 0) {
        st = 8;
    }
    if (camCtl.aiType != 0) {
        st = 5;
    }
    if (EvtSystemFlagGet(0x97) == 1 && st == 0) {
        return 0;
    }
    if (ChrPlayerIsBusy() == 1) {
        st = 4;
    }
    c = ChrGetByNo(1);
    if (c == NULL) {
        st = 9;
    } else if (c->moveCmds != 0) {
        st = 10;
    }
    if (st != 0) {
        mapMiniWork->showDelay = 1;
        return 1;
    }
    if (arg0 != 0 && mapMiniWork->showDelay < 0x3C) {
        mapMiniWork->showDelay++;
        return 1;
    }
    return 0;
}
