/* rodata: 0x00674C70-0x00674C80 */
#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/n22.h"

typedef struct {
    s32 idx;
    s32 val;
} Sel;


void AmapRollEncounterGroups(s32 count, FldMapSet *party, PtyEnemyParams *arg2, FldEncGroup *grps, FldCharDef *tbl) {
    Sel sel[4];
    s32 thr[4] = {100, 90, 70, 30};
    s32 j, k, m;
    s32 n;
    s32 total;
    s32 mode;
    s32 idx;
    s32 r;
    s32 lo;
    s32 range;
    s32 cost;
    s32 min, mi;
    FldEncGroup *g;
    u8 y;

    for (j = 0; j < 16; j++) {
        if (party->enc[j].defNo != 0) {
            if (party->enc[j].chance == 100) {
                break;
            }
        } else {
            party->enc[j].defNo = 1;
            if (party->enc[j].chance == 100) {
                break;
            }
        }
    }

    for (j = 0; j < count; j++) {
        total = 0;
        n = 0;
        k = 0;
        do {
            r = VuRandInt(100) + 1;
            if (thr[k] < r) {
                break;
            }
            mode = 0;
            if (grps[j].slots[k].defNo == 0) {
                r = VuRandInt(100) + 1;
                for (idx = 0; idx < 16; idx++) {
                    if (party->enc[idx].chance < r) {
                        continue;
                    }
                    mode = 1;
                    break;
                }
            } else {
                idx = grps[j].slots[k].defNo - 1;
                mode = 2;
            }

            if (mode != 0) {
                cost = tbl[party->enc[idx].defNo - 1].cost * party->enc[idx].count;
                sel[k].idx = idx;
                if (mode == 1) {
                    sel[k].val = party->enc[idx].order;
                } else {
                    sel[k].val = 0;
                }
            }
            if (mode == 0) {
                break;
            }
            if (n != 0 && total + cost >= 24) {
                break;
            }
            total += cost;
            n++;
            k++;
        } while (k < 4);
        for (k = 0; k < n; k++) {
            min = 0xFF;
            mi = 0;
            for (m = 0; m < n; m++) {
                if (sel[m].val < min) {
                    min = sel[m].val;
                    mi = m;
                }
            }
            idx = sel[mi].idx;
            sel[mi].val = 0xFF;
            lo = party->enc[idx].lvMin;
            range = party->enc[idx].lvMax - lo + 1;
            grps[j].slots[k].defNo = party->enc[idx].defNo - 1;
            grps[j].slots[k].kind = party->enc[idx].kind;
            grps[j].slots[k].count = party->enc[idx].count;
            grps[j].slots[k].level = VuRandInt(range) + lo;
        }
        for (k = n; k < 4; k++) {
            grps[j].slots[k].defNo = 0;
            grps[j].slots[k].kind = 0;
            grps[j].slots[k].count = 0;
            grps[j].slots[k].level = 0;
        }
        grps[j].cost = total;
        grps[j].count = n;
    }

    for (j = 0; j < count; j++) {
        g = &grps[j];
        m = 1;
        if (g->slots[0].kind == 0x25) {
            if (g->slots[1].defNo != 0) {
                m = 2;
            }
            y = g->slots[0].level;
            g->slots[m].defNo = AmapFindEncounterKind(0x26, party);
            g->slots[m].level = y;
            g->slots[m].count = 1;
            g->slots[m + 1].defNo = AmapFindEncounterKind(0x27, party);
            g->slots[m + 1].level = y;
            g->slots[m + 1].count = 1;
            g->count = m + 2;
        }
        if (g->slots[0].kind == 0x125) {
            if (g->slots[1].defNo != 0) {
                m = 2;
            }
            y = g->slots[0].level;
            g->slots[m].defNo = AmapFindEncounterKind(0x126, party);
            g->slots[m].level = y;
            g->slots[m].count = 1;
            g->slots[m + 1].defNo = AmapFindEncounterKind(0x127, party);
            g->slots[m + 1].level = y;
            g->slots[m + 1].count = 1;
            g->count = m + 2;
        }
        if (g->slots[0].kind == 0x23) {
            y = g->slots[0].level;
            g->slots[1].defNo = AmapFindEncounterKind(0x24, party);
            g->slots[1].level = y;
            g->slots[1].count = 1;
            g->slots[2].defNo = AmapFindEncounterKind(0x21, party);
            g->slots[2].level = y;
            g->slots[2].count = 3;
            g->slots[3].defNo = AmapFindEncounterKind(0x22, party);
            g->slots[3].level = y;
            g->slots[3].count = 1;
            g->count = 4;
        }
        if (g->slots[0].kind == 0x123) {
            y = g->slots[0].level;
            g->slots[1].defNo = AmapFindEncounterKind(0x124, party);
            g->slots[1].level = y;
            g->slots[1].count = 1;
            g->slots[2].defNo = AmapFindEncounterKind(0x121, party);
            g->slots[2].level = y;
            g->slots[2].count = 3;
            g->slots[3].defNo = AmapFindEncounterKind(0x122, party);
            g->slots[3].level = y;
            g->slots[3].count = 1;
            g->count = 4;
        }
    }
}
