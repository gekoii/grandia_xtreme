#include "common.h"
#include "battle/misc.h"

void PtclBurstUpdateAll(void) {
    PtclBurst *p = ptclBursts;
    s32 i;

    for (i = 0xF; i >= 0; i--) {
        if (p->cb30 != NULL) {
            p->cb30(p);
            p->unk4++;
        }
        p++;
    }
}
