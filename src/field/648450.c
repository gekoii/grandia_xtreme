/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniConcertSparksSpawn(u32 cnt, f32 x, f32 y) {
    u32 i;
    u32 n;
    MiniConcertSpark *s;

    i = 0;
    n = 0;
    for (; i < 128 && n < cnt; i++) {
        if (miniConcertFx->sparks[i].unk1C == 0) {
            s = &miniConcertFx->sparks[i];
            s->unk18 = 0;
            s->unk1A = miniConcertWork->song->unk1E;
            s->unk1D = 0;
            s->unk8 = x;
            s->unkC = y;
            s->unk1C = 1;
            s->unk10 = VuRandSigned(1.0f) * 3.14159f / 1.5f + -2.5f;
            s->unk14 = VuRandSigned(1.0f) + 0.5f;
            s->unk1E = VuRandInt(4) + 0x59;
            if (s->unk14 < 0.0f) {
                s->unk14 = -s->unk14;
            }
            n++;
        }
    }
}
