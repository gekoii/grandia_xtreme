#include "common.h"
#include "protos/main.h"
#include "main/n07.h"


FilePacModelPack *FilePacModelRelocate(FilePacModelPack *h) {
    u32 i;
    FilePacModelPack *g;
    u32 *t;
    if (h == NULL) {
        return NULL;
    }
    t = (u32 *)((u32)h + (u32)h->models);
    h->models = t;
    h->textures = (u32 *)((u32)h + (u32)h->textures);
    h->modelData = (u8 *)((u32)h + (u32)h->modelData);
    h->texData = (u8 *)((u32)h + (u32)h->texData);
    g = h;
    for (i = 0; i < g->modelCount; i++) {
        t[i] += (u32)g->modelData;
    }
    for (i = 0; i < g->texCount; i++) {
        g->textures[i] += (u32)g->texData;
    }
    for (i = 0; i < g->modelCount; i++) {
        FileChunkRelocate(g->models[i]);
    }
    for (i = 0; i < g->texCount; i++) {
        GphTexDataRelocate(g->textures[i]);
    }
    return g;
}
