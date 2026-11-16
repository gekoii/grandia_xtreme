/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "battle/n32.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }
#define SQ(x) ((x) * (x))

void BtlTargetFilterNearGroup(BtlTargetSel *s, s32 start, s32 n, f32 r) {
    s8 tmp[16];
    BtlUnit *me;
    BtlUnit *t;
    BtlUnit *u;
    s32 cnt;
    s32 i;

    BtlTargetPickByDistance(s, start, n, 0);
    me = s->self;
    t = &btlUnits[s->idx];
    r += fsqrt(SQ(me->pos[0] - t->pos[0]) + SQ(me->pos[2] - t->pos[2]));
    u = &btlUnits[start];
    cnt = 0;
    i = start;
    r *= r;
    for (; i < n; i++, u++) {
        if (s->flags[i] != 0) {
            if (SQ(me->pos[0] - u->pos[0]) + SQ(me->pos[2] - u->pos[2]) <= r) {
                tmp[i] = 1;
                cnt++;
                continue;
            }
        }
        tmp[i] = 0;
    }
    if (cnt != 0) {
        for (; start < n; start++) {
            s->flags[start] = tmp[start];
        }
    }
}
