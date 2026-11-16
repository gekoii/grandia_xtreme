#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"

void VuVecCopy(VuVec4 *, void *);

void FxStatusIconsDraw(u8 *arg0) {
    VuVec4 v;

    VuVecCopy(&v, arg0 + 0xC0);
    FxStatusIconDraw(arg0, 0, &v);
    FxStatusIconDraw(arg0, 7, &v);
    FxStatusIconDraw(arg0, 10, &v);
    FxStatusIconDraw(arg0, 1, &v);
    FxStatusIconDraw(arg0, 2, &v);
    FxStatusIconDraw(arg0, 3, &v);
    FxStatusIconDraw(arg0, 4, &v);
    FxStatusIconDraw(arg0, 5, &v);
    FxStatusIconDraw(arg0, 6, &v);
    FxStatusIconDraw(arg0, 8, &v);
}
