#include "common.h"
#include "ptcl.h"

s32 PtclEffectorFindIndex(s32 arg0) {
    PtclEffector *p = ptclCurSystem->effectors;
    s32 n;

    if (arg0 < 0) {
        return -1;
    }
    n = ptclCurSystem->effectorCount;
    while (n > 0) {
        if (p->id == arg0) {
            break;
        }
        n--;
        p++;
    }
    if (n <= 0) {
        return -1;
    }
    return ptclCurSystem->effectorCount - n;
}
