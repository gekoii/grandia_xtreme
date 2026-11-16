#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

void AmapNodesMark(void) {
    s32 i;

    for (i = 0; i < amapNodeCount; i++) {
        AmapCellSetNode(amapNodes[i].x, amapNodes[i].y);
    }
}
