#include "common.h"
#include "battle/misc.h"
#include "party.h"
#include "battle/n28.h"

void BtlDefeatAddExp(BtlUnit *ctx) {
    PtyUnit *st;
    BtlChrData *pt;
    s32 t;
    s32 i;
    s32 j;
    s16 *d;
    s32 *g;
    s8 *p;
    s16 *q;
    u8 k;

    g = &btlExpTotal;
    st = ctx->params;
    t = *g + st->exp;
    pt = ctx->def->data;
    *g = t;
    if (t >= 0) {
        if (t > 99999999) {
            t = 99999999;
        }
    } else {
        t = 0;
    }
    *g = t;
    q = st->expSkl;
    p = pt->expSklKind;
    for (i = 1; i >= 0; i--, q++, p++) {
        k = *p;
        if (k >= 1 && k <= 4) {
            btlElementExp[(s8)k - 1] += *q;
            j = *p - 1;
            d = &btlElementExp[j];
            *d = (*d < 0) ? 0 : ((*d >= 10000) ? 9999 : *d);
        }
    }
}
