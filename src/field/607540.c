#include "common.h"
#include "field/n21.h"

void AmapNodeAdd(s32 a, s32 b) {
    amapNodes[amapNodeCount].x = a;
    amapNodes[amapNodeCount].y = b;
    amapNodeCount++;
}
