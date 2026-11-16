#include "common.h"
#include "field/n15.h"

void GphPoly3DDraw(s32, void *, u32, s32);

void CamDebugTriDraw(void) {
    CamDebugTris *l = camDebugTris;
    s32 n = l->count;

    if (n != 0) {
        GphPoly3DDraw(n * 3, l->verts, 0x40000080, 0);
    }
}
