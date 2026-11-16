#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

void AmapStartFacingSet(AmapWork *a) {
    s32 v;

    AmapFloorMask8(a->startX, a->startY, &v);
    a->startFacing = (a->startFacing & 0x3F) | ((u8)AmapMaskToFacing(v) & 0xC0);
}
