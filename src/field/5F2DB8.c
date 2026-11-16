#include "common.h"
#include "party.h"

typedef struct {
    s16 v[6];
} Stat6;




void ItemGetEquipStats(s16 id, Stat6 *out) {
    PtyEquipData *e = ptyItemTable[id].equip;
    s32 i;

    if (e == NULL) {
        for (i = 0; i < 6; i++) {
            out->v[i] = 0;
        }
    } else {
        out->v[0] = e->stats[0];
        out->v[1] = e->stats[1];
        out->v[2] = e->stats[2];
        out->v[3] = e->stats[3];
        out->v[4] = e->stats[4];
        out->v[5] = e->stats[5];
    }
}
