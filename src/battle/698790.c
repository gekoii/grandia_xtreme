#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"


void FxCloneFree(void) {
    FxClone *p = &fxClone;
    s32 *h;
    s32 i;

    if (p->unit != 0) {
        if (p->pool != 0) {
            MemFree(p->pool);
            p->pool = 0;
        }
        h = p->drawLists;
        if (p->drawList != 0) {
            MemFree(p->drawList);
            p->drawList = 0;
        }
        i = 2;
        do {
            if (*h != 0) {
                MemFree(*h);
                *h = 0;
            }
            i--;
            h++;
        } while (i >= 0);
        EnvShadowRemoveCallback(p->shadowCb);
        p->unit = 0;
    }
}
