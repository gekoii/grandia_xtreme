#include "common.h"
#include "ptcl.h"




PtclEffector *PtclEffectorRegister(s32 arg0) {
    s32 n = ptclCurSystem->effectorCount;
    PtclEffector *e = ptclCurSystem->effectors;

    for (; n > 0; n--, e++) {
        if (e->id < 0 || e->id == arg0) {
            break;
        }
    }
    if (n <= 0) {
        return NULL;
    }
    e->id = arg0;
    return e;
}
