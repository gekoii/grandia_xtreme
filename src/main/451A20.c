#include "common.h"
#include "ptcl.h"





s32 MemMove128(void *, void *, s32);
void VuMtxMul(FMATRIX *, s32, FMATRIX *);
s8 GphMeshClipTest(GphGaObject *);
void GphMeshDrawClip(GphGaObject *, s32);

void PtclEmitterDraw(PtclEmitter *o, s32 arg1) {
    FMATRIX m;
    s32 i;
    s32 n;
    s8 r;
    GphRenderCtx *perf;

    n = 0;
    if (o->unk12B9 != 0) {
        MemMove128(ptclEmitterDrawCtx, &ptclEmitterDrawTemplate, 8);
        ptclEmitterDrawCtx->ambient = (f32 *)&o->colorCur;
        ptclEmitterDrawCtx->verts = (FVECTOR *)o->verts;
        ptclEmitterDrawCtx->radius = 2.0f * o->unk12A8;
        ptclEmitterDrawCtx->colors = o->colorTag;
        ptclEmitterDrawCtx->mtx2 = &m;
        ptclEmitterDrawCtx->mtx = (s32)&m;
        for (i = 0; i < o->pointCount; i++) {
            VuMtxMul(&m, arg1, &o->points[i]);
            r = GphMeshClipTest(ptclEmitterDrawCtx);
            if (r >= 0) {
                ptclEmitterDrawCtx->clip = r;
                GphMeshDrawClip(ptclEmitterDrawCtx, 0);
                n++;
            }
        }
    }
    perf = gphRenderCtx;
    perf->unk14 += n * 0x3E;
    perf->unk18 += n * 0x7C;
}
