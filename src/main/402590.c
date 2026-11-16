#include "common.h"
#include "protos/main.h"
#include "ptcl.h"


PtclStarterEntry *PtclStarterUpdate(void) {
    PtclStarterEntry *e;
    PtclStarterEntry *found;
    s32 i;
    s32 v;

    found = NULL;
    if (ptclStarterFrame < 0 || ptclPtbData == NULL) {
        return NULL;
    }
    e = ptclStarterTable;
    for (i = 0; i < ptclPtbData->starterCount; i++, e++) {
        if (e->id != ptclStartMaskId) {
            if (e->time == ptclStarterFrame) {
                v = e->ctrl;
                if (v < 0) {
                    found = e;
                } else if (v < ptclPtbData->ctrlCount) {
                    PtclStarterSpawn(i, v, e);
                }
            } else if (e->time < ptclStarterFrame && ptclStarterInst[i] != NULL) {
                if (PtclGenIsAlive((s32)ptclStarterInst[i]->gen) == 0) {
                    PtclStarterKill(i);
                }
            }
        }
    }
    ptclStarterFrame++;
    return found;
}
