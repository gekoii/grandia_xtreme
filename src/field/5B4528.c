#include "common.h"
#include "field/event.h"
#include "field/n12.h"

extern OccHandler occEndTable[];

void OccEnd(OccEntry *e) {
    OccHandler f = occEndTable[e->kind];

    if (f != NULL) {
        f(e);
    }
    e->state = OCC_SLEEP;
    e->flags &= 0xFFCF;
}
