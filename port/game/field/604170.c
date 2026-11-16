/* Port-only C for AmapTemplateListsBuild (0x00604170), which the decompilation still has in assembly. */
#include "common.h"
#include "field/n21.h"

typedef struct AmapTemplateList {
    /* 0x00 */ u8 count;
    /* 0x01 */ u8 ids[15];
} AmapTemplateList; /* size 0x10 */

extern AmapTemplateList amapTemplateListTable[10][7][3];
extern const u8 amapTypeTierTable[8];

/* Candidate lists of areas 0-8: list 0 gets the templates of the tiers
   below the map type's tier, list 1 those of its tier; kinds 1 and 2 only
   when unk112/unk110 allow them. Area 9's lists are copied whole, count
   included (list 1 takes its count from the second byte of the area's
   first entry, as the game does). The table is read as bytes, so a count
   over 15 runs into the next entry like on the PS2. */
void AmapTemplateListsBuild(AmapWork *w) {
    const u8 *tbl = (const u8 *)amapTemplateListTable;
    u8 tiers[8];
    AmapTmplCand *c;
    s32 tier;
    s32 i;
    s32 j;
    s32 t;
    s32 k;
    s32 n;
    s32 base;
    s32 c0;
    s32 c1;

    for (i = 0; i < 8; i++) {
        tiers[i] = amapTypeTierTable[i];
    }
    tier = tiers[w->mapType];

    for (i = 0; i < 9; i++) {
        c1 = 0;
        c0 = 0;
        for (j = 0; j < tier; j++) {
            for (t = 0; t < 3; t++) {
                if (t == 1 && w->unk112 == 0) {
                    continue;
                }
                if (t == 2 && w->unk110 == 0) {
                    continue;
                }
                base = (i * 21 + j * 3 + t) * 16;
                n = tbl[base];
                for (k = 0; k < n; k++) {
                    c = &w->tmplLists[i].cands[0][0] + c0 + 1;
                    c->id = tbl[base + 1 + k];
                    c->kind = t;
                    c0++;
                }
            }
        }
        for (t = 0; t < 3; t++) {
            if (t == 1 && w->unk112 == 0) {
                continue;
            }
            if (t == 2 && w->unk110 == 0) {
                continue;
            }
            base = (i * 21 + tier * 3 + t) * 16;
            n = tbl[base];
            for (k = 0; k < n; k++) {
                c = &w->tmplLists[i].cands[1][0] + c1 + 1;
                c->id = tbl[base + 1 + k];
                c->kind = t;
                c1++;
            }
        }
        w->tmplLists[i].cands[0][0].id = c0;
        w->tmplLists[i].cands[1][0].id = c1;
    }

    base = 9 * 21 * 16;
    n = tbl[base];
    for (j = 0; j < n + 1; j++) {
        (&w->tmplLists[9].cands[0][0] + j)->id = tbl[base + j];
    }
    n = tbl[base + 1];
    for (j = 0; j < n + 1; j++) {
        (&w->tmplLists[9].cands[1][0] + j)->id = tbl[base + 16 + j];
    }
}
