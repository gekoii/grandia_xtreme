#include "common.h"
#include "field/event.h"
#include "field/n12.h"

extern OccHandler occBeginTable[];

void OccCallBegin(OccEntry *e) {
    OccHandler f = occBeginTable[e->kind];

    if (f != NULL) {
        f(e);
    }
}
