#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n22.h"

typedef struct {
    u8 pad0[0x60];
    s32 unk60;
} Sub;

typedef struct {
    s32 count;
    u8 pad4[0x24 - 4];
    Sub **unk24;
} Hdr;


void AmapLoadCellModels(s32 arg0) {
    Hdr *h;
    Sub *s;
    s32 n;
    s32 i;
    GphModel *o;

    h = FilePacModelRelocate(arg0);
    FilePacModelBindTextures(h);
    s = *h->unk24;
    n = h->count;
    if (s != NULL) {
        i = 0;
        amapState->texId = s->unk60;
        amapState->cellModelCount = n;
        if (n > 0) {
            do {
                o = ChrModelCreate(FilePacGetModel(h, i));
                amapCellModels[i] = o;
                ChrModelSetPartPrio(o, 0);
                amapCellModels[i]->flags = 0;
                i++;
            } while (i < n);
        }
    }
}
