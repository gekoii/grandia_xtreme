#include "common.h"
#include "field/event.h"
#include "field/n12.h"

extern OccHandler occBackwardStartTable[];

void OccCallBackwardStart(OccEntry *e) {
    OccHandler f = occBackwardStartTable[e->kind];

    if (f != NULL) {
        f(e);
    }
}
