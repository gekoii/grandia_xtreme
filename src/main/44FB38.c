#include "common.h"
#include "ptcl.h"




void GphModelUpdateGsRegs(GphModel *);

void PtclModelsUpdateGsRegs(void) {
    s32 n;
    PtclGen **pp;
    PtclGen *o;
    s32 cnt;
    GphModel **ids;
    PtclModelPart *e;

    n = ptclCurSystem->genCount;
    pp = ptclCurSystem->gens;
    for (; n > 0; n--, pp++) {
        o = *pp;
        if (o == NULL) {
            continue;
        }
        if (o->liveCount <= 0) {
            continue;
        }
        if (o->models == NULL) {
            continue;
        }
        ids = o->models;
        cnt = o->groupCount;
        e = o->modelParts;
        for (; cnt > 0; cnt--, e++, ids++) {
            if (e->life != 0) {
                GphModelUpdateGsRegs(*ids);
            }
        }
    }
}
