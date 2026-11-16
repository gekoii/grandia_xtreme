/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n28.h"


void BtlDropObjRegister(void) {
    BtlDropObj *p;
    BtlDropCoin *q;
    s32 i;
    s32 n;

    p = btlDropObjs;
    for (i = 0; i < 12; i++, p++) {
        if (p->kind > 0 && !(p->alpha <= 0.0f)) {
            if (p->kind < 3) {
                n = p->coinCount;
                q = p->coins;
                while (n > 0) {
                    n--;
                    ChrModelSetAlpha(q->model, (p->alpha - 1.0f) * 128.0f);
                    BtlChrModelCalcDraw(q->model);
                    q++;
                }
            } else {
                ChrModelSetAlpha(p->model, (p->alpha - 1.0f) * 128.0f);
                BtlChrModelCalcDraw(p->model);
            }
        }
    }
}
