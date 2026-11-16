/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniConcertBurstSpawn(u8 big, f32 x, f32 y) {
    u32 i;
    u32 n;
    u32 max;
    f32 sc;
    MiniConcertBurst *b;

    n = 0;
    if (big == 0) {
        sc = 3.0f;
        max = 50;
    } else {
        sc = 1.0f;
        max = 20;
    }
    for (i = 0; i < 128; i++) {
        if (miniConcertFx->bursts[i].unk7C == 0) {
            b = &miniConcertFx->bursts[i];
            b->unk78 = 0;
            b->unk7A = 30;
            b->unk7C = 1;
            b->unk7D = 0;
            b->unk68 = x;
            b->unk6C = y;
            b->unk70 = VuRandSigned(1.0f) * 6.28318f;
            b->unk74 = (VuRandSigned(1.0f) + 0.5f) * 5.0f - 0.5f;
            b->unk7E = VuRandInt(4) + 0x5E;
            if (b->unk74 < 0.0f) {
                b->unk74 = -b->unk74;
            }
            if (b->unk74 < 1.0f) {
                b->unk74 = b->unk74 + 2.0f;
            }
            n++;
            b->unk74 = b->unk74 * sc;
            if (n > max) {
                break;
            }
        }
    }
}
