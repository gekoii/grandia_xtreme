#include "common.h"
#include "protos/main.h"
#include "gph.h"

extern u8 gphShatterLightBlock[];

/* Matching needs part->mtx read as an s32 and the GA object's +0x3C
 * (lightBlock) written as a pointer: the casts keep those alias sets. */
void GphShatterLoad(s32 arg0) {
    GphModel *list;
    GphPart *e;
    s32 i;

    ChrModelPoolSet(gphShatter.modelPool);
    FileChunkRelocate(arg0);
    list = ChrModelCreate(arg0 + 0x10);
    gphShatter.model = list;
    e = list->parts;
    e--;
    list->flags = 0;
    for (i = 0; i < gphShatter.model->partCount; i++) {
        e++;
        if (e->gaObjs != NULL) {
            e->gaObjs->mtx = *(s32 *)&e->mtx;
            *(void **)&e->gaObjs->lightBlock = gphShatterLightBlock;
            e->gaObjs->ambient = gphShatter.color;
            e->gaObjs->mtx2 = (void *)*(s32 *)&e->mtx;
            e->flags = 0;
        }
    }
}
