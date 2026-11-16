#include "common.h"
#include "ptcl.h"

s32 EffListAnyVisible(PtclEmitter **arg0, s32 arg1) {
    s32 i;

    if (arg0 == NULL) {
        return 0;
    }
    for (i = 0; i < arg1; i++) {
        if (arg0[i] != NULL && arg0[i]->unk12B9 != 0) {
            return 1;
        }
    }
    return 0;
}
