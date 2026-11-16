#include "common.h"
#include "battle/n28.h"

void FxHitInit(void) {
    FxHitSlot *p;
    s32 i;
    s32 m;

    m = -1;
    p = fxHitSlots;
    for (i = 15; i >= 0; i--) {
        p->type = 0;
        p->modelBuf = 0;
        p->unk24 = 0;
        p->draw = 0;
        p->update = 0;
        p->light = m;
        p++;
    }
}
