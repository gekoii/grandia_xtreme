#include "common.h"
#include "ptcl.h"

void PtclSystemSelect(s32 arg0) {
    ptclCurSystem = (PtclSystem *)arg0;
}

s32 PtclSystemGetCur(void) {
    return (s32)ptclCurSystem;
}
