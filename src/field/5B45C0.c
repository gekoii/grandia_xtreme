#include "common.h"
#include "field/event.h"
#include "field/n12.h"

extern OccHandler occForwardTable[];

void OccCallForward(OccEntry *e) {
    OccHandler f = occForwardTable[e->kind];

    if (f != NULL) {
        f(e);
    }
}
