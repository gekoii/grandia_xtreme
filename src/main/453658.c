#include "common.h"
#include "protos/main.h"
#include "main/n07.h"

typedef struct {
    u8 pad[0x60];
    s64 idx;
} Ent;

typedef struct {
    u8 pad[3];
    u8 b3;
    u8 pad4[0x7C];
} Prim;

typedef struct {
    u8 pad[4];
    u32 count;
    u8 pad8[0x4C];
    Prim *prims;
} Grp;


void FilePacModelBindTextures(FilePacModelPack *arg0) {
    u8 tbl[256];
    u32 i;
    u32 j;
    Ent *e;
    s32 idx;
    u8 r;
    Grp *g;
    Prim *p;

    if (arg0 == NULL) {
        return;
    }
    for (i = 0; i < arg0->texCount; i++) {
        e = (Ent *)arg0->textures[i];
        idx = e->idx;
        r = GphTexRegister(e);
        tbl[idx] = r;
        e->idx = tbl[idx];
    }
    tbl[255] = 0;
    for (i = 0; i < arg0->modelCount; i++) {
        g = FilePacGetModel(arg0, i);
        p = g->prims;
        for (j = 0; j < g->count; j++, p++) {
            p->b3 = tbl[p->b3];
        }
    }
}
