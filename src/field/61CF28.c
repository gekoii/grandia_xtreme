#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/n22.h"

typedef struct {
    s16 id;
    u8 pad2[0xC];
} Mem;

typedef struct {
    u8 pad0[2];
    s16 unk2;
    u8 pad4[0x16];
    Mem mem[4];
    u8 pad52[0xC0 - 0x52];
} Src;

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void FldEncGroupSetup(u32 count, Src *src, u32 n) {
    FldMapSet *party;
    u32 i;
    s32 j;
    s32 size;
    FldEncGroup *g;
    Mem *m;
    FldMapSetEnc *e;
    s32 id;
    Src *s;

    if (count == 0) {
        return;
    }
    party = MapSetFileGet();
    if (party == NULL) {
        return;
    }
    size = count * 0x1A;
    fldEncGroups.count = count;
    fldEncGroups.groups = FldMemAlloc(size);
    MemSet(fldEncGroups.groups, 0, size);
    for (i = 0; i < n; i++) {
        s = &src[i];
        g = &fldEncGroups.groups[i];
        if (s->unk2 != 1 && s->mem[0].id > 0) {
            g->slots[0].defNo = s->mem[0].id;
        }
    }
    AmapRollEncounterGroups(fldEncGroups.count, party, ptyBattleParams, fldEncGroups.groups, fldCharDefTable);
    for (i = 0; i < fldEncGroups.count; i++) {
    }
    if (src == NULL) {
        return;
    }
    for (i = 0; i < n; i++) {
        if (src[i].unk2 == 1) {
            g = &fldEncGroups.groups[i];
            g->count = 0;
            for (j = 0; j < 4; j++) {
                g->slots[j].defNo = 0;
                g->slots[j].count = 0;
                g->slots[j].level = 0;
            }
            for (j = 0; j < 4; j++) {
                m = &src[i].mem[j];
                id = m->id;
                if (id != 0) {
                    e = &party->enc[id - 1];
                    g->slots[g->count].defNo = e->defNo - 1;
                    g->slots[g->count].count = e->count;
                    g->slots[g->count].level = e->lvMin + VuRandInt(e->lvMax - e->lvMin);
                    g->count++;
                } else {
                    g->slots[g->count].defNo = 0;
                    g->slots[g->count].count = 0;
                    g->slots[g->count].level = 0;
                }
            }
        }
    }
}
