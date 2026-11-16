#include "common.h"
#include "protos/field.h"
#include "field/n22.h"


void AmapGenBuild(AmapWork *m) {
    s32 i;

    if (m->result != 0) {
        return;
    }
    if (m->tiles == NULL) {
        return;
    }
    if (m->cells == NULL) {
        return;
    }
    i = 0;
    AmapFloorAdvance(m);
    for (;;) {
        AmapGenerate(m);
        if (m->result == 0) {
            break;
        }
        i++;
        m->result = 0;
        if (i >= 11) {
            amapForceAlgo1 = 1;
            amapForceAlgo2 = 1;
            AmapGenerate(m);
            amapForceAlgo1 = -1;
            amapForceAlgo2 = -1;
            break;
        }
    }
    m->mapSet->monsterCount = 0;
}
