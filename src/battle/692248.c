#include "common.h"
#include "battle/n29.h"

void BtlCmdListMultiCastNext(void) {
    BtlCmdList *p = &btlCmdList;
    BtlCmdListCell *c;
    BtlCmdListCellAt *v;
    s32 i;
    s32 j;
    s32 found;
    s32 id;
    s8 k;

    p->rowState[p->row] = -2;
    p->alive = -1;
    for (i = 0; i < p->count; i++) {
        if (p->rowState[i] >= 0) {
            found = 0;
            c = &p->cells[i * 6];
            for (j = 0; j < 6; j++) {
                if (c->flags != 0 && !(c->flags & 2)) {
                    if (p->unit->skillId == c->id) {
                        found = 1;
                    } else {
                        c->flags |= 2;
                    }
                }
                c++;
            }
            if (!found) {
                p->rowState[i] = -2;
            } else {
                p->alive++;
            }
        }
    }
    if (p->alive >= 0) {
        do {
            p->row++;
            if (p->row >= p->count) {
                p->row = 0;
            }
        } while (p->rowState[k = p->row] < 0);
        p->unk1C = 1;
        p->col = 0;
        id = p->unit->skillId;
        k = p->row * 6;
        v = BTL_CMD_LIST_CELL_AT(p, k);
        while (v->cell.id != id) {
            v = BTL_CMD_LIST_CELL_AT(v, 1);
            k++;
        }
        p->col = k % 6;
    }
}
