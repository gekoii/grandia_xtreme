#include "common.h"
#include "main/n03.h"

s32 strncmp(char *, char *, s32);
extern u8 ctypeTable[];

void McSlotsMarkUsed(McDirList *list) {
    s32 i;
    s32 n;
    sceMcTblGetDir *e;
    s32 c0;
    s32 c1;
    McSlotTable *t;
    McSlot *s;

    t = &mcSlotTable;
    s = t->slots;
    n = t->count;
    for (i = 0; i < n; i++) {
        s->used = 0;
        s->status = 0;
        s->unk2 = 0;
        s++;
    }
    n = list->count;
    for (i = 0; i < n; i++) {
        e = &list->entries[i];
        if (strncmp(e->EntryName, mcSaveNamePrefix, 4) == 0) {
            c0 = (u8)e->EntryName[4];
            c1 = (u8)e->EntryName[5];
            if ((ctypeTable[c0] & 4) && (ctypeTable[c1] & 4)) {
                { s32 idx = (c0 - '0') * 10 + (c1 - '0'); ((McSlotTable *)((u8 *)&mcSlotTable + idx * sizeof(McSlot)))->slots[0].used = 1; }
            }
        }
    }
}
