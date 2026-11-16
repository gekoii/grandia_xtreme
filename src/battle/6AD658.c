#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"


s32 PtclBtlSprUpdate(PtclBtlSpr *p, s32 n, s32 type) {
    s32 cnt = 0;
    s32 *s = ptclBurstTypeTable[type].animSet;
    s32 a = 0;

    if (s != NULL) {
        a = *s;
    }
    GphAnimSetSelect(a);
    for (; n > 0; n--, p++) {
        if (p->life > 0) {
            p->life--;
            p->pos[0] += p->vel[0];
            p->pos[1] += p->vel[1];
            p->pos[2] += p->vel[2];
            p->vel[0] += p->accel[0];
            p->vel[1] += p->accel[1];
            if (type == 11 || type == 12) {
                p->vel[1] -= 0.2f;
            }
            p->vel[2] += p->accel[2];
            if (type == 16) {
                if (!(btlTickCount & 1)) {
                    GphAnimStep(&p->anim, 1.0f);
                }
            } else {
                GphAnimStep(&p->anim, 1.0f);
            }
            cnt++;
        }
    }
    return cnt;
}
