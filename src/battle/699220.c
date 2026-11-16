#include "common.h"
#include "battle/n29.h"
#include "protos/battle.h"

void FxAtkBuild(void) {
    FxAtk *p;
    s32 i;

    GphDrawListSet(btlSysDrawList);
    i = 32;
    p = fxAtkPool;
    do {
        if (p->flags && p->dead == 0 && p->build && (p->flags & 0x200)) {
            p->build(p);
        }
        i--;
        p++;
    } while (i > 0);

    GphDrawListSet(btlFxDrawList);
    i = 32;
    p = fxAtkPool;
    do {
        if (p->flags && p->dead == 0 && p->build && !(p->flags & 0x200)) {
            p->build(p);
        }
        i--;
        p++;
    } while (i > 0);
}
