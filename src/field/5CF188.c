#include "common.h"
#include "field/n15.h"

void *CamScrGet(s32 arg0) {
    CamScrTable *t = camScrTable;
    /* + arg0 - arg0 is not dead for the match: with q = t the function is one insn shorter (12 words off) */
    CamScrTable *q = (CamScrTable *)((u8 *)t + arg0 - arg0);

    if (t == NULL) {
        return NULL;
    }
    if (arg0 >= t->count) {
        return NULL;
    }
    if (arg0 < 0) {
        return NULL;
    }
    return (u8 *)t + q->offs[arg0];
}
