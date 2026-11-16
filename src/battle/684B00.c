/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n28.h"
#include "protos/battle.h"

void BtlDropShadowPrepare(void) {
    BtlDropObj *p;
    BtlDropCoin *q;
    s32 i;
    s32 n;

    p = btlDropObjs;
    for (i = 0; i < 12; i++, p++) {
        if (p->kind > 0 && !(p->alpha <= 0.0f)) {
            GphDrawListSet(p->drawList);
            GphDrawListClear(NULL);
            if (p->kind < 3) {
                n = p->coinCount;
                q = p->coins;
                while (n > 0) {
                    n--;
                    GphModelDrawSimple(q->model);
                    q++;
                }
            } else {
                GphModelDrawSimple(p->model);
            }
            GphDrawListIndex();
        }
    }
}
