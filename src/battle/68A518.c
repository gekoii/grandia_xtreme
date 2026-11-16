/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/misc.h"
#include "protos/battle.h"

void PtclBtlDustUpdate(void) {
    s32 i;
    s32 **pp;
    PtclBtlDust *p;
    s32 j;

    pp = ptclBtlDustGroups;
    for (i = 0; i < 32; i++) {
        if (*pp != NULL) {
            switch (**pp) {
            case 0x80:
                PtclBtlDustRingUpdate(i);
                break;
            case 0x83:
                PtclBtlDustSprayUpdate(i);
                break;
            }
        }
        pp++;
    }
    p = ptclBtlDustPool;
    for (j = 0; j < 128; j++) {
        if (!(p->unk0 <= 0.0f) && p->unkC > 0) {
            switch (p->unkC) {
            case 1:
                PtclBtlDustPuffUpdate(p);
                break;
            case 2:
                PtclBtlDustCloudUpdate(p);
                break;
            }
        }
        p++;
    }
}
