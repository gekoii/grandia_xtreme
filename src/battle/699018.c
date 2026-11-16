#include "common.h"
#include "battle/n29.h"

void FxAtkInit(void) {
    FxAtk *p;
    s32 i;

    i = 32;
    p = fxAtkPool;
    do {
        p->flags = 0;
        i--;
        p++;
    } while (i > 0);
}
