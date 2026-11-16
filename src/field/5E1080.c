#include "common.h"
#include "ptcl.h"

void PtclFldEffectStop(s32 arg0) {
    ptclFldWork->stop[arg0 & 0xFFFF] = 1;
}
