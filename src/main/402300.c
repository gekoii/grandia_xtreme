#include "common.h"
#include "protos/main.h"
#include "ptcl.h"


s32 PtclStarterKill(s32 arg0) {
    PtclStarterSlot *o;
    s32 h;

    o = ptclStarterInst[arg0];
    if (o != NULL) {
        h = (s32)o->gen;
        if (h != 0) {
            if (PtclGenRelease(h) == 0) {
                return 0;
            }
            MemFree(h);
        }
        PtclStarterSlotFree((s32 *)ptclStarterInst[arg0]);
        ptclStarterInst[arg0] = NULL;
        ptclCurWork -= ptclStarterWorkSize[arg0];
    }
    return 1;
}
