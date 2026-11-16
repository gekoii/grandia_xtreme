/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

extern s8 sndSoftVoiceTick;

void SndSoftVoiceFetch(s32 *arg0) {
    SndSoftVoice *s;
    s32 i;
    s32 a;
    s32 d;

    s = sndSoftVoices;
    for (i = 7; i >= 0; i--) {
        SndSoftFetchSamples(s->pos, arg0++);
        a = s->pos + 0x40;
        s->pos = a;
        if (sndSoftVoiceTick != 0) {
            if (a >= s->end) {
                s->pos = a - (s->end - s->loopStart);
                if (s->loop == 0) {
                    s->env = s->unk2C = 0.0f;
                    s->state = 0;
                }
            }
            if (s->state == 2) {
                s->env = s->env * s->decay;
                if (s->env < 0.004f) {
                    s->env = s->unk2C = 0.0f;
                    s->state = 0;
                }
            }
        } else {
            if (s->unk24 < s->unk26) {
                s->unk24++;
            }
            if (s->unk26 < s->unk24) {
                s->unk24--;
            }
            d = (s->panTarget - s->pan) & 0xFF;
            if (d > 0x80) {
                s->pan--;
            } else if (d != 0) {
                s->pan++;
            }
            s->pan &= 0xFF;
        }
        s++;
    }
    sndSoftVoiceTick ^= 1;
}
