#include "common.h"
#include "main/n06.h"

void GphSphereSetDivisions(u32 arg0, u32 arg1) {
    gphSphereSlices = (arg0 <= 32) ? arg0 : 32;
    gphSphereStacks = (arg1 <= 32) ? arg1 : 32;
}
