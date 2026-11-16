#include "common.h"
#include "ptcl.h"

void PtclEffectorRemove(s32 arg0) {
    s32 n;
    PtclEffector *e;

    e = ptclCurSystem->effectors;
    if (arg0 >= 0) {
        n = ptclCurSystem->effectorCount;
        while (n > 0) {
            n--;
            if (e->id == arg0) {
                e->id = -1;
                return;
            }
            e++;
        }
    }
}
