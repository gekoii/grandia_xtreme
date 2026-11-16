#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"


void SysMfifoStop(s32 arg0) {
    GsMfifoTerm();
}
