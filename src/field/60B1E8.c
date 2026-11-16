#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

void AmapNodesMaskSet(void) {
    s32 i;
    s32 v;

    for (i = 0; i < amapNodeCount; i++) {
        AmapFloorMask8(amapNodes[i].x, amapNodes[i].y, &v);
        amapNodes[i].unk4 = 0;
        amapNodes[i].floorMask = v;
    }
}
