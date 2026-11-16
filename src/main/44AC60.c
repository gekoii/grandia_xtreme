#include "common.h"
#include "ptcl.h"
#include "protos/main.h"




s32 PtclGenRelease(PtclGen *arg0) {
    if (arg0 == NULL) {
        return 1;
    }
    if (arg0->liveCount < 0) {
        if (arg0->frame != SysGetFrameCount()) {
            ptclCurSystem->gens[arg0->index] = 0;
            return 1;
        }
    } else {
        arg0->liveCount = -1;
        arg0->frame = SysGetFrameCount();
    }
    return 0;
}
