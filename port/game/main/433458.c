/* Port-only C for GphModelCalcViewMatrices (0x00433458), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "protos/main.h"
#include "vu0_port.h"

/* Scratchpad copy of the view matrix */
#define SPR_VIEW_MTX ((f32 *)0x70003100)

void VuMtxMul(f32 *dst, f32 *a, f32 *b);

void GphModelCalcViewMatrices(GphModel *model, f32 *view);

/*
 * model->viewMtx = partCount matrices from the pool, each the part's rest
 * matrix times the view matrix (staged in the scratchpad). The asm's loop
 * is a do-while: it runs once even for partCount 0.
 */
void GphModelCalcViewMatrices(GphModel *model, f32 *view) {
    s32 n = model->partCount;
    f32 *dst = (f32 *)VuMtxPoolAllocN(n);
    f32 *src;

    model->viewMtx = (FMATRIX *)dst;
    VuCopyMtx(SPR_VIEW_MTX, view);
    src = model->restMtx;
    do {
        VuMtxMul(dst, SPR_VIEW_MTX, src);
        dst += 16;
        src += 16;
    } while (--n > 0);
}
