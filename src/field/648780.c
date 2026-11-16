#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniConcertSparksUpdate(void) {
    u32 i;
    MiniConcertSpark *s;
    f32 k;
    f32 k2;
    f32 t;
    s16 n;
    s32 m;

    for (i = 0; i < 128; i++) {
        s = &miniConcertFx->sparks[i];
        if (s->unk1C != 0) {
            k = s->unk18;
            k = k * cosf(s->unk10);
            s->unk0 = s->unk8 + k * s->unk14;
            k2 = s->unk18;
            k2 = k2 * sinf(s->unk10);
            s->unk4 = s->unkC + k2 * s->unk14;
            t = logf(s->unk18 * 0.8f + 1.0f);
            s->unk0 += cosf(s->unk10) * t * 20.0f;
            t = logf(s->unk18 * 0.8f + 1.0f);
            s->unk4 += sinf(s->unk10) * t * 20.0f;
            switch (s->unk1C) {
            case 0:
                break;
            case 1:
                n = s->unk1D;
                if (n == 30) {
                    s->unk1D = 0;
                    n = 0;
                    s->unk1C = 2;
                }
                goto tail;
            case 2:
                m = s->unk1D;
                if (m == miniConcertWork->song->unk1C) {
                    s->unk1D = 0;
                    n = 0;
                    s->unk1C = 4;
                } else {
                    n = m;
                }
                goto tail;
            case 4: {
                s32 m4 = s->unk1D;
                if (m4 == s->unk1A) {
                    s->unk18 = 0;
                    s->unk1D = 0;
                    s->unk1C = 0;
                    break;
                }
                n = m4;
            }
            tail:
                s->unk1D = n + 1;
                s->unk18++;
                break;
            }
        }
    }
}
