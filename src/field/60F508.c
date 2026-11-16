#include "common.h"
#include "protos/field.h"

int abs(int);

s32 AmapGridDist(s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 dx = abs(x0 - x1);
    s32 dy = abs(y0 - y1);
    return sqrt(dx * dx + dy * dy);
}
