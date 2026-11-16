#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void FxAfterimageFree(void) {
    FxAfterimage *p = &fxAfterimage;
    s32 *h;
    s32 i;

    if (p->unit != NULL) {
        h = p->drawLists;
        i = 7;
        MemFree(p->poses);
        p->poses = 0;
        do {
            if (*h != 0) {
                MemFree((void *)*h);
                *h = 0;
            }
            i--;
            h++;
        } while (i >= 0);
        p->unit = 0;
    }
}
