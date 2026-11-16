#include "common.h"
#include "field/event.h"

void EvtAreaFindByIdNoResult(s32 id) {
    s32 n;
    s32 i;
    EvtArea *e;

    n = evtAreaTable->count;
    /* goto loop: for/do-while forms differ by 5 words (counter increment placement) */
    i = 0;
    if (n > 0) {
        e = evtAreaTable->areas;
    loop:
        i++;
        if (e->id != id) {
            e++;
            if (i < n) {
                goto loop;
            }
        }
    }
}
