#include "common.h"
#include "field/event.h"
#include "field/n12.h"

extern OccHandler occForwardStartTable[];

void OccCallForwardStart(OccEntry *e) {
    OccHandler f = occForwardStartTable[e->kind];

    if (f != NULL) {
        f(e);
    }
}
