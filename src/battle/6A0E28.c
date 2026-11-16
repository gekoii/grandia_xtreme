#include "common.h"
#include "battle/misc.h"
#include "battle/n30.h"

void FxCastAuraSet(s32 *arg0, s8 arg1) {
    FxCastAura *p = &fxCastAura[*arg0];
    s8 cur = p->kind;

    if (cur != arg1) {
        if (arg1 < 0) {
            if (p->release == 0) {
                if (cur == 2) {
                    p->release = cur;
                }
                if (cur == 1) {
                    p->release = cur;
                }
            }
        } else {
            p->kind = arg1;
            p->state = 0;
            p->stamp = btlFrameCount;
            if (p->pool == 0) {
                p->stamp = btlFrameCount + 1;
            }
            p->model = NULL;
            p->release = 0;
        }
    }
}
