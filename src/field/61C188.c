#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n22.h"

extern void *fldSpotMotionPack;

void FldSpotLoadModels(FldSpotHdr *h) {
    u32 i;
    FldSpotDef *e;

    h->modelPack = h->modelPack + (s32)h;
    h->motionPack = h->motionPack + (s32)h;
    h->entries = (FldSpotDef *)((s32)h + (s32)h->entries);
    for (i = 0; i < h->count; i++) {
        e = (FldSpotDef *)((i << 5) + (s32)h->entries);
        if (e->inst == -1) {
            e->inst = e->model;
        }
    }
    fldSpotWork->hdr = h;
    fldSpotModelPack = FilePacModelRelocate(h->modelPack);
    if (fldSpotModelPack != NULL) {
        FilePacModelBindTextures(fldSpotModelPack);
    }
    fldSpotMotionPack = FilePacMotionRelocate(h->motionPack);
    for (i = 0; i < h->motionCount; i++) {
        fldSpotWork->motTable.slots[i] = FilePacGetMotion(fldSpotMotionPack, i);
    }
}
