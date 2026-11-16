#include "common.h"
#include "protos/field.h"
#include "field/n21.h"


void AmapGenBuildOnce(AmapWork *arg0) {
    if (arg0->result == 0 && arg0->tiles != NULL && arg0->cells != NULL) {
        AmapFloorAdvance(arg0);
        AmapGenerate(arg0);
    }
}
