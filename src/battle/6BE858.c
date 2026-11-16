/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "party.h"
#include "protos/battle.h"

typedef struct {
    f32 x;
    f32 z;
} Pos2;

extern const Pos2 btlFormationSpots[];

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

#define SQ(x) ((x) * (x))

void BtlFormationFindSpot(BtlUnit *self, f32 *out, s32 mode) {
    f32 best[5];
    s32 sel[5];
    s32 c80;
    s32 i, j, k;
    s32 cnt;
    BtlUnit *u;
    f32 m;
    f32 d;
    f32 mx;
    s32 n;

    for (i = 0; i < 5; i++) {
        best[i] = -9999.0f;
        c80 = 0x80;
        for (j = 0; j < 5; j++) {
            m = 9999.0f;
            u = btlUnits;
            for (k = 0; k < 16; k++, u++) {
                if (u->state > 0 && u->body < 0) {
                    if (u->state == c80 && u->phase == 0) {
                        d = fsqrt(SQ(u->moveTarget[0] - btlFormationSpots[i * 5 + j].x) + SQ(u->moveTarget[2] - btlFormationSpots[i * 5 + j].z));
                    } else {
                        d = fsqrt(SQ(u->pos[0] - btlFormationSpots[i * 5 + j].x) + SQ(u->pos[2] - btlFormationSpots[i * 5 + j].z));
                    }
                    d -= fsqrt(SQ(self->params->colR + u->params->colR));
                    if (d <= best[i]) {
                        m = d;
                        k = 16;
                    } else if (d < m) {
                        m = d;
                    }
                }
            }
            if (best[i] < m) {
                best[i] = m;
                sel[i] = i * 5 + j;
            }
        }
    }
    i = 0;
    if (mode == 1) {
        cnt = 0;
        for (i = 0; i < 5; i++) {
            if (best[i] >= 0.0f) {
                cnt++;
            }
        }
        if (cnt == 0) {
            mx = best[0];
            n = sel[0];
            for (i = 1; i < 5; i++) {
                if (mx < best[i]) {
                    mx = best[i];
                    n = i;
                }
            }
            i = n;
        } else {
            for (i = 0; i < 4; i++) {
                if (best[i] >= 0.0f) {
                    if ((f32)(rand() >> 15) * (1.0f / 65536.0f) <= 1.0f / (f32)cnt) {
                        break;
                    }
                    cnt--;
                }
            }
        }
        *out++ = btlFormationSpots[sel[i]].x;
        *out = btlFormationSpots[sel[i]].z;
        return;
    }
    for (i = 0; i < 5; i++) {
        *out++ = btlFormationSpots[sel[i]].x;
        *out++ = btlFormationSpots[sel[i]].z;
    }
}
