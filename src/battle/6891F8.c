#include "common.h"
#include "protos/battle.h"
#include "battle/n28.h"


void FxHitFree(FxHitSlot *e) {
    if (e->modelBuf != 0) {
        MemFree(e->modelBuf);
        e->modelBuf = 0;
    }
    e->unk24 = 0;
    e->draw = 0;
    e->update = 0;
    e->type = 0;
}
