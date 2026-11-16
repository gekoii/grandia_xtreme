#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"


void FxCastAuraFree(FxCastAura *arg0) {
    if (arg0->pool != 0) {
        MemFree(arg0->pool);
        arg0->pool = 0;
    }
    arg0->kind = 0;
}
