#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/n28.h"


void ChrPartDispSet(BtlUnit *u, s16 id, s8 flag) {
    ChrPartDispSlot *s = &chrPartDispTable[u->id];
    s32 i;

    if (u->model == NULL) {
        return;
    }
    if (ChrModelFindPart(u->model, id) == NULL) {
        return;
    }
    for (i = 0; i < 16; i++) {
        if (s->ids[i] == id) {
            break;
        }
    }
    if (flag < 0) {
        if (i < 16) {
            s->ids[i] = 0;
        }
    } else {
        if (i >= 16) {
            for (i = 0; i < 16; i++) {
                if (s->ids[i] <= 0) {
                    break;
                }
            }
            if (i >= 16) {
                return;
            }
        }
        s->ids[i] = id;
        s->flags[i] = flag != 0;
    }
    ChrPartDispApply(u);
}
