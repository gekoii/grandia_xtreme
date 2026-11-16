#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

void AmapFloorIndexReset(void) {
    amapFloorIndex = 0;
    AmapSetFloorIndex(0);
}
