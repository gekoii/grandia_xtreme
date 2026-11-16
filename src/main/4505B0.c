#include "common.h"
#include "main/n07.h"
#include "protos/main.h"

void FlushCache(s32);

void PtclDrawStrip(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4) {
    ptclDrawCtx.verts = (FVECTOR *)arg1;
    if (arg2 != NULL) {
        ptclDrawCtx.colors = arg2;
    } else {
        ptclDrawCtx.colors = ptclStripColors;
    }
    if (arg3 != NULL) {
        ptclDrawCtx.uvs = arg3;
    } else {
        ptclDrawCtx.uvs = ptclStripUvs;
    }
    ptclDrawCtx.mtx2 = (void *)arg0;
    ptclDrawCtx.mtx = arg0;
    if (arg4 != NULL) {
        ptclDrawCtx.ambient = arg4;
    } else {
        ptclDrawCtx.ambient = ptclDefaultColor;
    }
    FlushCache(0);
    GphVu1Render(&ptclDrawCtx, 0);
}
