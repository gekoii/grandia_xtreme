#include "common.h"
#include "battle/misc.h"

void PtclBurstInit(void) {
    PtclBurst *p = ptclBursts;
    s32 i;

    for (i = 0xF; i >= 0; i--) {
        p->type = 0;
        p->cb38 = 0;
        p->cb34 = 0;
        p->cb30 = 0;
        p->parts2 = 0;
        p->parts = 0;
        p->unk2C = 0;
        p++;
    }
}
