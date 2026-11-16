#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void CamTrailDebugDraw(void) {
    s32 i = 0;
    VuVec4 *p = (VuVec4 *)0x70003000;
    s32 n = CamTrailCount() + 2;

    for (; i < n; i++) {
        CamTrailGet(i, p);
        p->y -= 100.0f;
        p->w = 1.0f;
        p++;
    }
    GphLine3DDraw(n, (VuVec4 *)0x70003000, 0x80200080, 1);
}
