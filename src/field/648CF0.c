/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniConcertBurstsUpdate(void) {
    u32 i;
    MiniConcertBurst *b;
    f32 a;
    f32 k;
    f32 k2;

    miniConcertFx->unk9802 = 0;
    for (i = 0; i < 128; i++) {
        b = &miniConcertFx->bursts[i];
        if (b->unk7C != 0) {
            if (b->unk78 == b->unk7A) {
                b->unk78 = 0;
                b->unk7C = 0;
            } else {
                a = 1.0f - (f32)b->unk78 / (f32)b->unk7A;
                k = b->unk7D;
                k = k * cosf(b->unk70);
                b->unk60 = b->unk68 + k * b->unk74;
                k2 = b->unk7D;
                k2 = k2 * sinf(b->unk70);
                b->unk64 = b->unk6C + k2 * b->unk74 + (f32)b->unk7D * 0.1f * (f32)b->unk7D;
                b->unk7D++;
                b->spr.a = a * 128.0f;
                b->unk78++;
                b->spr.ofsX = b->unk60;
                b->spr.ofsY = b->unk64;
                miniConcertFx->unk9802 = 1;
            }
        }
    }
}
