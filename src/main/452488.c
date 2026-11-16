#include "common.h"
#include "protos/main.h"
#include "sys.h"


s32 TxtFontImageCalcSize(TxtFontImage *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 v1;
    s32 a0;

    if (arg0 == NULL) {
        return 0;
    }
    GsTexInfoInit(arg0->texDim, arg0->pixels, arg0->clut, arg3, arg4);
    v1 = arg0->texDim[0];
    a0 = arg0->texDim[1];
    arg0->cellW = arg1;
    arg0->cellH = arg2;
    arg0->w = v1;
    arg0->h = a0;
    arg0->pixelSize = (v1 / 2) * a0;
    return arg0->pixelSize;
}
