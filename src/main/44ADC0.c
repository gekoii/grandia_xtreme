#include "common.h"
#include "ptcl.h"

s32 PtclGenIsAlive(PtclGen *arg0) {
    if (arg0 == NULL) {
        return 0;
    }
    if (arg0->liveCount < 0) {
        return 0;
    }
    if (arg0->liveCount == 0 && ((arg0->flags & 0x400) || arg0->def.rate + arg0->def.rateRange < 0.0009f)) {
        return 0;
    }
    return 1;
}
