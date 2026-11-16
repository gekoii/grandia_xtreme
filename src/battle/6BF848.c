/* cc: ee-gcc2.9-991111-01 */
/* cflags: -fno-strength-reduce */
/* -fno-strength-reduce: the parts[] loops index (with it on, they walk pointers) */
#include "common.h"
#include "battle/unit.h"

#define SET(u, v, n)                                    \
    if ((n) <= 0) {                                     \
        (u)->alpha = (v);                               \
        (u)->alphaStep = 0.0f;                          \
    } else {                                            \
        (u)->alphaStep = ((v) - (u)->alpha) / (f32)(n); \
    }

void BtlUnitFadeTo(BtlUnit *arg0, s32 mode, s32 n, f32 v) {
    BtlUnit *u;
    BtlUnit *o;
    BtlUnit *w;
    s32 i;
    s32 k;
    s32 j;
    s32 c;
    u32 bit;

    if (mode == 7) {
        w = btlUnits;
        for (i = 15; i >= 0; i--, w++) {
            if (w->state > 0) {
                SET(w, v, n);
            }
        }
        return;
    }
    if (arg0 == NULL) {
        return;
    }
    u = btlUnits;
    for (bit = 1; bit < 0x10000; bit <<= 1, u++) {
        if (btlTargetMask & bit) {
            if ((mode & 2) && u->state > 0) {
                SET(u, v, n);
                if (u->body >= 0) {
                    o = &btlUnits[u->body];
                    SET(o, v, n);
                }
            }
        } else if ((mode & 4) && u->id != arg0->id && u->state > 0 && u->comboLeader != arg0->id) {
            if (u->parts[0] >= 0) {
                k = 0;
                do {
                    c = u->parts[k];
                    if (c >= 0) {
                        if (c == arg0->id) {
                            break;
                        }
                        if (((s32)btlTargetMask >> c) & 1) {
                            break;
                        }
                    }
                    k++;
                } while (k < 3);
                if (k < 3) {
                    continue;
                }
            }
            SET(u, v, n);
        }
    }
    if (mode & 1) {
        SET(arg0, v, n);
        if (arg0->body >= 0) {
            o = &btlUnits[arg0->body];
            SET(o, v, n);
        }
        if (arg0->parts[0] >= 0) {
            for (j = 0; j < 3; j++) {
                c = arg0->parts[j];
                if (c >= 0) {
                    o = &btlUnits[c];
                    SET(o, v, n);
                }
            }
        }
    }
}
