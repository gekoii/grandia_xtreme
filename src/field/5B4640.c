#include "common.h"
#include "field/event.h"
#include "field/n12.h"

extern OccHandler occBackwardTable[];

void OccCallBackward(OccEntry *e) {
    OccHandler f = occBackwardTable[e->kind];

    if (f == NULL) {
        e->state = OCC_SLEEP;
        e->flags &= 0xFFCF;
    } else {
        f(e);
    }
}
