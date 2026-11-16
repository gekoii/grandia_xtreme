#include "common.h"
#include "protos/field.h"
#include "field/n21.h"


void AmapDeadEndsBranch(s32 arg0) {
    s32 i;
    s32 n;
    s32 x;
    s32 y;
    s32 v;

    for (i = 0; i < amapNodeCount; i++) {
        x = amapNodes[i].x;
        y = amapNodes[i].y;
        if (AmapCorridorNeighbourCount(x, y, &v, arg0) < 2) {
            n = 0;
            while (i == VuRandInt(amapNodeCount) && ++n < 10) {
            }
            AmapBranchWalk(x, y, v, arg0);
        }
    }
}
