#include "common.h"
#include "protos/field.h"
#include "field/n21.h"


void AmapNodesLinkRandom(s32 arg0) {
    s32 i;
    s32 n;
    s32 a;
    s32 b;
    s32 x;
    s32 y;
    s32 ay;

    n = amapNodeCount;
    for (i = 0; i < n; i++) {
        a = VuRandInt(amapNodeCount);
        b = VuRandInt(amapNodeCount);
        ay = amapNodes[a].y;
        x = amapNodes[b].x;
        y = amapNodes[b].y;
        AmapDistFillFree(amapNodes[a].x, ay, arg0);
        AmapCorridorTrace(x, y);
    }
}
