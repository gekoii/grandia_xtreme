#include "common.h"
#include "battle/n34.h"

void DbgBtlHitDispInit(void) {
    s32 i;
    DbgBtlHitMark *m;

    dbgBtlHitDispFlags = 0;
    dbgBtlHitMarkNext = 0;
    m = dbgBtlHitMarks;
    i = 0xF;
    do {
        m->alpha = 0;
        i -= 1;
        m++;
    } while (i >= 0);
}
