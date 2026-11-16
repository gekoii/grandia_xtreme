#include "common.h"
#include "battle/misc.h"

void PtclBurstDrawAll(void) {
    PtclBurst *p = ptclBursts;
    s32 i;

    for (i = 0xF; i >= 0; i--, p++) {
        if (p->cb38 != NULL) {
            p->cb38(p);
        }
    }
}
