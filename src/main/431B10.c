/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "camera.h"
#include "protos/main.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void CamCalcShake(Camera *cam) {
    FVECTOR v;
    f32 frames;
    f32 d;
    f32 r;
    u32 t;
    u32 n;
    /* Copies of cam that copy propagation folds away: they only set the
     * instruction count gcse sizes its hash table by, which orders the
     * spill slots of the PRE temporaries. */
    Camera *c0;
    Camera *c1;
    Camera *c2;
    Camera *c3;
    u32 i;
    f32 a;
    f32 b;

    frames = (u32)cam->shakePeriod;
    t = cam->shakeTick;
    c0 = cam;
    c1 = cam;
    c2 = cam;
    c3 = cam;
    if (cam->shakeMode & 8) {
        for (i = 0; i < 3; i++) {
            if (frames <= 0.0f) {
                b = cam->shakeAmp2[i];
                cam->shakeAmp[i] = b;
                cam->shakeMode &= ~8;
                cam->shakeRand[i] = cam->shakeRand2[i];
            } else {
                b = cam->shakeAmp2[i];
                a = cam->shakeAmp[i];
                d = b - a;
                if (fabsf(d) > 0.1f) {
                    a += d / frames;
                    cam->shakeAmp[i] = a;
                }
                b = cam->shakeRand2[i];
                a = cam->shakeRand[i];
                d = b - a;
                if (fabsf(d) > 0.1f) {
                    a += d / frames;
                    cam->shakeRand[i] = a;
                }
            }
        }
    }
    copyq(v, cam->shakeAmp);
    if (c3->shakeMode & 2) {
        for (i = 0; i < 3; i++) {
            if (fmodf(t, cam->shakeCycle[i]) < 0.1f) {
                cam->unkD0[i] = VuRandSigned(v[i] * 2.0f);
            }
            v[i] = cam->unkD0[i];
        }
    } else {
        for (i = 0; i < 3; i++) {
            if (fmodf(t, cam->shakeCycle[i]) <= 0.1f) {
                r = cam->shakeRand[i];
                r = VuRandSigned(r) + r * 0.5f;
                cam->unkD0[i] = v[i] + r;
            }
            v[i] = cam->unkD0[i] * sinf(t * 3.141592f / cam->shakeCycle[i] + cam->unk120[i]);
        }
    }
    t++;
    n = cam->shakeCycle[0] * cam->shakeCycle[1] * cam->shakeCycle[2];
    if (n < t) {
        t -= n;
    }
    if (frames > 0.0f) {
        c2->shakePeriod = (u32)(frames - 1.0f);
    }
    c0->shakeTick = t;
    cam->shakeOfs[0] = v[0];
    c1->shakeOfs[1] = v[1];
    cam->shakeOfs[2] = v[2];
}
