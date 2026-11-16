#include "common.h"
#include "protos/battle.h"

s32 BtlEntry(void) {
    SysProgRegister(BtlInit, BtlStart, BtlPrepare, BtlUpdate,
                  BtlBuildDrawLists, BtlDraw, BtlModelCalcPass, DbgBtlTopMenu);
    return 1;
}
