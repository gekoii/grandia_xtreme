#include "common.h"
#include "m2c_macros.h"
#include "ptcl.h"

void PtclSetMainPool(s32 arg0) {
    ptclMainPool = (PtclSystem *)arg0;
}
