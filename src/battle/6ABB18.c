#include "common.h"
#include "battle/misc.h"

void PtclBurstRegisterAll(void) {
    PtclBurst *p = ptclBursts;
    s32 i;

    for (i = 0xF; i >= 0; i--, p++) {
        if (p->cb34 != NULL) {
            p->cb34(p);
        }
    }
}
