#include "common.h"
#include "main/n10.h"

extern s8 sndPanDelayTable[];
extern s32 sndSoftMixBuf[];

void SndSoftMix(s32 *in, s16 *out) {
    SndSoftVoice *v;
    s32 *pa;
    s32 *pb;
    s32 i;
    s32 d;
    s32 x;
    s32 t;
    u16 pan;

    v = sndSoftVoices;
    for (i = 0; i < 8; i++) {
        t = sndPanDelayTable[v->pan];
        pan = v->pan;
        pa = &sndSoftMixBuf[v->delayL];
        d = t - v->delayL;
        if (d != 0) {
            if (d > 0) {
                *pa++ += v->lastL;
                *pa++ += v->lastL;
                v->delayL++;
            } else {
                v->delayL--;
            }
        } else {
            *pa++ += v->lastL;
        }
        pb = &sndSoftMixBuf[0x40 + v->delayR];
        d = sndPanDelayTable[0x100 - (s16)pan] - v->delayR;
        if (d != 0) {
            if (d > 0) {
                *pb++ += v->lastR;
                *pb++ += v->lastR;
                v->delayR++;
            } else {
                v->delayR--;
            }
        } else {
            *pb++ += v->lastR;
        }
#define A(n) pa[n] += in[(n) * 8];
#define B(n) pb[n] += in[(n + 32) * 8];
        A(0) A(1) A(2) A(3) A(4) A(5) A(6) A(7) A(8) A(9) A(10) A(11) A(12) A(13) A(14) A(15)
        A(16) A(17) A(18) A(19) A(20) A(21) A(22) A(23) A(24) A(25) A(26) A(27) A(28) A(29) A(30)
        B(0) B(1) B(2) B(3) B(4) B(5) B(6) B(7) B(8) B(9) B(10) B(11) B(12) B(13) B(14) B(15)
        B(16) B(17) B(18) B(19) B(20) B(21) B(22) B(23) B(24) B(25) B(26) B(27) B(28) B(29) B(30)
        v->lastL = in[31 * 8];
        v->lastR = in[63 * 8];
        in++;
        v++;
    }
    pa = sndSoftMixBuf;
    for (d = 0; d < 32; d++) {
        x = pa[0x40];
        if (x > 0x6400) {
            x = (x >> 5) + 0x60E0;
        }
        if (x < -0x6400) {
            x = (x >> 5) - 0x60E0;
        }
        out[0x100] = x;
        x = pa[0];
        if (x > 0x6400) {
            x = (x >> 5) + 0x60E0;
        }
        if (x < -0x6400) {
            x = (x >> 5) - 0x60E0;
        }
        *out++ = x;
        pa[0x40] = pa[0x60];
        pa[0] = pa[0x20];
        pa[0x60] = 0;
        pa[0x20] = 0;
        pa++;
    }
}
