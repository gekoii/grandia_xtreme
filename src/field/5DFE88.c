#include "common.h"

s32 PtclEmitterUpdate(void *, s32);

void EffObjUpdate(void *arg0, s32 arg1) {
    if (arg0) {
        PtclEmitterUpdate(arg0, arg1);
    }
}
