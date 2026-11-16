#include "common.h"
#include "battle/n29.h"

void FxAfterimageSetColors(s32 xx, s32 yy) {
    FxAfterimage *p = &fxAfterimage;
    if (p->unit != NULL && p->count > 0) {
        u8 *xp = (u8 *)&xx;
        u8 *yp = (u8 *)&yy;
        p->color0[0] = xp[3];
        p->color0[1] = xp[2];
        p->color0[2] = xp[1];
        p->color0[3] = xp[0];
        p->color1[0] = yp[3];
        p->color1[1] = yp[2];
        p->color1[2] = yp[1];
        p->color1[3] = yp[0];
    }
}
