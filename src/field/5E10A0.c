#include "common.h"
#include "protos/field.h"
#include "ptcl.h"






void MemSet(s32, s32, s32);

s32 PtclFldEffectRelease(u16 idx) {
    PtclPtbHeader *t;
    PtclStarterEntry *e;
    s32 i;
    s32 ok;

    if (ptclFldWork->res[idx] == NULL) {
        return 1;
    }
    t = ptclFldWork->res[idx]->ptb;
    if (t == NULL) {
        return 1;
    }
    ok = 1;
    for (i = 0; i < t->starterCount; i++) {
        e = (PtclStarterEntry *)((u8 *)t + t->starterOffset) + i;
        if (e->id == -1) {
            continue;
        }
        if (e->ctrl < 0) {
            continue;
        }
        if (e->ctrl >= t->ctrlCount) {
            continue;
        }
        if (PtclGenRelease(ptclFldWork->slots[idx][e->ctrl]->gen) == 1) {
            MemSet((s32)ptclFldWork->slots[idx][e->ctrl]->gen, 0, 4);
        } else {
            ok = 0;
        }
    }
    return ok;
}
