#include "common.h"
#include "protos/battle.h"
#include "battle/n28.h"


void BtlDropObjFree(BtlDropObj *o) {
    if (o->modelBuf != 0) {
        MemFree(o->modelBuf);
        o->modelBuf = 0;
    }
    if (o->drawList != 0) {
        MemFree(o->drawList);
        o->drawList = 0;
    }
    if (o->coins != NULL) {
        MemFree((s32)o->coins);
        o->coins = NULL;
    }
    o->kind = 0;
}
