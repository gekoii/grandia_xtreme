/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "battle/misc.h"

void PtclBtlDustInit(void) {
    PtclBtlDust *p;
    s32 *q;
    s32 i;

    p = ptclBtlDustPool;
    for (i = 127; i >= 0; i--) {
        p->unk0 = 0;
        p++;
    }
    q = ptclBtlDustGroups;
    for (i = 31; i >= 0; i--) {
        *q = 0;
        q++;
    }
}
