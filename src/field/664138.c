#include "common.h"
#include "field/event.h"

void *EvtScnrFind(u16 id) {
    u16 *t = evtScnrWork->index;
    u16 *p;
    u16 key;
    u16 val;

    if (t == NULL) {
        return NULL;
    }
    p = t;
    for (;;) {
        key = *p++;
        val = *p++;
        if (val == 0xFFFF) {
            return NULL;
        }
        if (key == id) {
            return evtScnrWork->body + val * 8;
        }
    }
}
