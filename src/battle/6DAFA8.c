#include "common.h"
#include "battle/n34.h"

void DbgBtlHitMarkFade(void) {
    s32 i;
    DbgBtlHitMark *m;
    u8 a;

    i = 0xF;
    m = dbgBtlHitMarks;
    do {
        a = m->alpha;
        i -= 1;
        if (a != 0) {
            m->alpha = a - 4;
        }
        m++;
    } while (i >= 0);
}
