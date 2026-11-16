#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n22.h"

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

typedef struct {
    u8 pad0[0xC];
    FldSpotPlaceTable *tbl;
} Sec;

typedef struct {
    u8 pad0[4];
    Sec *sec;
} Blob;

void *FldMemAlloc(s32);
void MemSet(void *, s32, s32);

void FldSpotSetup(Blob *blob) {
    ChrObj obj;
    FldMapSet *ev;
    FldSpotPlace *spots;
    void *src;
    FldSpotDef *a;
    FldSpot *e;
    s32 i;
    s32 j;
    s32 r;
    u16 id;

    ev = MapSetFileGet();
    if (ev == NULL) {
        return;
    }
    blob->sec = (Sec *)((u8 *)blob + (u32)blob->sec);
    blob->sec->tbl = (FldSpotPlaceTable *)((u8 *)blob + (u32)blob->sec->tbl);
    spots = blob->sec->tbl->spots;
    fldSpotWork->list = FldMemAlloc(blob->sec->tbl->count * sizeof(FldSpot));
    fldSpotWork->itemIds = FldMemAlloc(ev->spotItems * 4);
    MemSet(fldSpotWork->list, 0, blob->sec->tbl->count * sizeof(FldSpot));
    MemSet(fldSpotWork->itemIds, 0, ev->spotItems * 4);
    AmapRollItems(ev->spotItems, 2, ev, fldSpotWork->itemIds);
    fldSpotWork->count = blob->sec->tbl->count;
    for (i = 0; i < blob->sec->tbl->count; i++) {
        fldSpotWork->order[i] = 0xFF;
    }
    j = 0;
    /* goto retry: an inner do-while for the free-slot search lays the loop out differently */
    do {
    retry:
        r = VuRandInt(blob->sec->tbl->count);
        if (fldSpotWork->order[r] != 0xFF) {
            goto retry;
        }
        fldSpotWork->order[r] = j;
        j++;
    } while (blob->sec->tbl->count != j);
    fldSpotWork->modelCount = *fldSpotModelPack;
    for (i = 0; i < *fldSpotModelPack; i++) {
        fldSpotWork->models[i] = ChrModelCreate(FilePacGetModel(fldSpotModelPack, i));
        fldSpotWork->models[i]->flags |= 8;
    }
    src = FilePacGetModel(fldSpotModelPack, 0);
    for (i = 0; i < blob->sec->tbl->count; i++) {
        fldSpotWork->list[i].model = ChrModelCreate(src);
        copyq(fldSpotWork->list[i].model->pos, spots[fldSpotWork->order[i]].pos);
        copyq(fldSpotWork->list[i].model->scale, spots[fldSpotWork->order[i]].scale);
        fldSpotWork->list[i].model->rot[0] = spots[fldSpotWork->order[i]].rot[0];
        fldSpotWork->list[i].model->rot[1] = spots[fldSpotWork->order[i]].rot[1];
        fldSpotWork->list[i].model->rot[2] = spots[fldSpotWork->order[i]].rot[2];
        fldSpotWork->list[i].unkA = 1;
    }
    fldSpotWork->blob = blob;
    j = 0;
    for (i = 0; i < ev->spotItems; i++) {
        fldSpotWork->list[j].kind = 1;
        fldSpotWork->list[j].unk9 = 0;
        j++;
        if (j == blob->sec->tbl->count) {
            return;
        }
    }
    for (i = 0; i < ev->spotTraps; i++) {
        fldSpotWork->list[j].kind = 3;
        fldSpotWork->list[j].unk9 = 2;
        j++;
        if (j == blob->sec->tbl->count) {
            return;
        }
    }
    for (i = 0; i < ev->spotHeals; i++) {
        fldSpotWork->list[j].kind = 4;
        fldSpotWork->list[j].unk9 = 2;
        j++;
        if (j == blob->sec->tbl->count) {
            return;
        }
    }
    for (i = 0; i < ev->spotMonsters; i++) {
        fldSpotWork->list[j].kind = 2;
        fldSpotWork->list[j].unk9 = 1;
        j++;
        if (j == blob->sec->tbl->count) {
            return;
        }
    }
    for (i = j; i < blob->sec->tbl->count; i++) {
        fldSpotWork->list[i].kind = 0;
        fldSpotWork->list[i].unk9 = VuRandInt(3);
        if (j == blob->sec->tbl->count) {
            return;
        }
    }
    obj.motTable = &fldSpotWork->motTable;
    for (i = 0; i < blob->sec->tbl->count; i++) {
        e = (FldSpot *)(i * sizeof(FldSpot) + (u32)fldSpotWork->list);
        a = (FldSpotDef *)((e->unk9 << 5) + (u32)fldSpotWork->hdr->entries);
        if (a->motion2 != -1) {
            id = a->motion;
            obj.model = e->model;
            ChrMotSetRandomFrame(&obj, id);
        }
    }
}
