/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/battle.h"
#include "battle/n32.h"

void BtlTargetPickByStat(BtlTargetSel *s, s32 i, s32 n, void *field, s32 mode) {
    BtlUnit *u;
    s32 off;
    s32 max;
    f32 v;
    f32 best;

    max = mode < 0x100;
    best = -1e8f;
    off = (u8 *)field - (u8 *)s->self->params;
    if (!max) {
        best = 1e8f;
    }
    u = &btlUnits[i];
    for (; i < n; i++, u++) {
        if (s->flags[i] != 0) {
            switch (mode & 0xFF) {
            case 0:
                v = *(s32 *)((u8 *)u->params + off);
                break;
            case 1:
                v = *(s16 *)((u8 *)u->params + off);
                break;
            case 2:
                v = *(s8 *)((u8 *)u->params + off);
                break;
            case 3:
                v = *(u32 *)((u8 *)u->params + off);
                break;
            case 4:
                v = *(u16 *)((u8 *)u->params + off);
                break;
            case 5:
                v = *(u8 *)((u8 *)u->params + off);
                break;
            default:
                v = *(f32 *)((u8 *)u->params + off);
                break;
            }
            v += (rand() >> 15) * (1.0f / 65536.0f) * 0.005f;
            if (!max ? v < best : best < v) {
                s->idx = i;
                best = v;
            }
        }
    }
}
