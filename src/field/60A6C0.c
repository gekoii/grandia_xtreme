#include "common.h"
#include "protos/field.h"
#include "field/n21.h"


void AmapNodesLinkChain(s32 arg) {
    s32 i;

    for (i = 0; i < amapNodeCount - 1; i++) {
        AmapDistFillFree(amapNodes[i].x, amapNodes[i].y, arg);
        VuRandInt(amapNodeCount - 1);
        AmapCorridorTrace(amapNodes[i + 1].x, amapNodes[i + 1].y);
        do { } while (0); /* empty do-while(0): scheduling barrier, i++ stays after the calls */
    }
}
