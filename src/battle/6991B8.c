#include "common.h"
#include "battle/n29.h"

void FxAtkDraw(void) {
    FxAtk *p;
    s32 i;

    i = 32;
    p = fxAtkPool;
    do {
        i--;
        if (p->flags && p->dead == 0 && p->draw) {
            p->draw(p);
        }
        p++;
    } while (i > 0);
}
