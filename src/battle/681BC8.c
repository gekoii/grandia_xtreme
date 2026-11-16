/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "battle/n28.h"

void FxHomingPoolClear(FxHomingPool *p) {
    FxHoming *e = p->entries;
    s32 n;

    if (e != NULL) {
        for (n = p->count; n > 0; n--, e++) {
            e->state = 0;
        }
    }
}
