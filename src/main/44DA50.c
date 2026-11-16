#include "common.h"
#include "protos/main.h"
#include "ptcl.h"



extern void PtclSpriteEmit(PtclGen *);
extern void PtclModelEmit(PtclGen *);

void PtclUpdate(void) {
    PtclGen **p;
    s32 n;

    PtclCtrlTracksUpdate();
    n = ptclCurSystem->genCount;
    for (p = ptclCurSystem->gens; n > 0; n--, p++) {
        if (*p != NULL && (*p)->liveCount >= 0) {
            if ((*p)->models == 0) {
                PtclSpriteEmit(*p);
                if ((*p)->liveCount > 0) {
                    PtclSpriteMove(*p);
                }
            } else {
                PtclModelEmit(*p);
                if ((*p)->liveCount > 0) {
                    PtclModelMove(*p);
                }
            }
            (*p)->frame++;
        }
    }
}
