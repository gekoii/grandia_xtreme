#include "common.h"
#include "battle/n28.h"

void FxHitUpdate(void) {
    FxHitSlot *p;
    s32 i;

    p = fxHitSlots;
    for (i = 15; i >= 0; i--, p++) {
        if (p->update != NULL) {
            p->update(p);
        }
    }
}
