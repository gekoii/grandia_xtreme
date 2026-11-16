#include "common.h"
#include "battle/n28.h"

s32 FxHomingPoolCount(FxHomingPool *p) {
    FxHoming *e = p->entries;
    s32 n;
    s32 c = 0;

    if (e == NULL) {
        return 0;
    }
    for (n = p->count; n > 0; n--, e++) {
        if (e->state != 0) {
            c++;
        }
    }
    return c;
}
