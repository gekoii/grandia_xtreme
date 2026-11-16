#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/n28.h"


void ChrPartDispApply(BtlUnit *u) {
    GphModel *m = u->model;
    GphPart *dst;
    GphPart *src;
    ChrPartDispSlot *s;
    s32 i;
    s32 n;

    if (m != NULL) {
        dst = m->parts;
        src = u->def->modelPack->parts;
        for (n = m->partCount; n > 0; n--) {
            dst->flags = (dst->flags & ~8) | (src->flags & 8);
            src++;
            dst++;
        }
        s = &chrPartDispTable[u->id];
        for (i = 0; i < 16; i++) {
            if (s->ids[i] > 0) {
                ChrPartDispSetOne(u, s->ids[i], s->flags[i]);
            }
        }
    }
}
