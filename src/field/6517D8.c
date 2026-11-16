/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

void MiniBallAddVelocity(MiniBall *, f32, f32);
f32 VuVecNormalizeLen(f32 *, f32 *);

#define VADD(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define VSUB(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vsub.xyzw $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

#define copyq(dst, src) \
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void MiniBallBallsUpdate(MiniBall *cels, u8 n) {
    MiniBallGame *s = &miniBallWork->game;
    FVECTOR d;
    f32 speed;
    f32 zero;
    u32 i;
    u32 j;

    if (s->judged == 1) return;
    MiniBallCollideBalls(cels, n);
    speed = miniBallData->params->pullZ;
    MiniBallStickPush();
    MiniBallShakeUpdate();
    switch (miniBallData->set->gustMode) {
    case 1:
        MiniBallRandomGusts();
        break;
    case 2:
        MiniBallScriptedGusts();
        break;
    }
    for (i = 0; i < n; i++) {
        if (cels[i].state == 0) continue;
        switch (cels[i].state) {
        case 1:
            cels[i].vel[2] += speed;
            VADD(cels[i].pos, cels[i].pos, cels[i].vel);
            if (cels[i].vel[2] > 0.0f) {
                cels[i].state = 2;
                cels[i].pos[0] = miniBallData->params->dropX + VuRandSigned(5.0f);
                cels[i].sprite->layer = 0;
            }
            break;
        case 2:
            cels[i].vel[2] += speed;
            VADD(cels[i].pos, cels[i].pos, cels[i].vel);
            if (miniBallData->params->dropZ < cels[i].pos[2]) {
                cels[i].state = 3;
            }
            break;
        case 3:
            for (j = 0; j < n; j++) {
                if (cels[j].state == 4) {
                    VSUB(d, cels[i].pos, cels[j].pos);
                    if (fsqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]) < 10.0f) break;
                }
            }
            if (j == n) {
                cels[i].state = 4;
                cels[i].travel = cels[i].speed = VuVecNormalizeLen(cels[i].vel, cels[i].vel);
            }
            break;
        case 4:
            zero = 0.0f;
            MiniBallAddVelocity(&cels[i], zero, speed);
            MiniBallMove(&cels[i]);
            if (cels[i].pos[0] > 600.0f || cels[i].pos[0] < zero) {
                cels[i].state = 0;
            }
            if (cels[i].pos[2] > 400.0f) {
                cels[i].state = 0;
            } else if (cels[i].pos[2] < 0.0f) {
                cels[i].state = 0;
            }
            break;
        case 5:
            cels[i].vel[2] += speed;
            if (miniBallData->params->winEndZ < cels[i].pos[2]) {
                copyq(cels[i].vel, miniBallWork->vec0);
                cels[i].state = 4;
                cels[i].travel = cels[i].speed = VuVecNormalizeLen(cels[i].vel, cels[i].vel);
            } else {
                VADD(cels[i].pos, cels[i].pos, cels[i].vel);
            }
            break;
        }
        cels[i].sprite->ofsX = cels[i].pos[0] - 10.0f;
        cels[i].sprite->ofsY = cels[i].pos[2] - 10.0f;
    }
    /* nested ifs: an && of the two byte tests is folded into one ld (MiniBallGame is 16-aligned); the original tests them separately */
    if (s->ballHitSe == 1) {
        if (s->ballHitWait == 0) {
            SndSePlay(0x813, 0, 0, s->ballHitVol, 0);
            s->ballHitVol = 0xFF;
            s->ballHitWait = 3;
            s->ballHitSe = 0;
        }
    }
    if (s->ballHitWait != 0) s->ballHitWait--;
    if (s->wallHitSe == 1) {
        if (s->wallHitWait == 0) {
            SndSePlay(0x80A, 0, 0, s->wallHitVol, 0);
            s->wallHitVol = 0xFF;
            s->wallHitWait = 3;
            s->wallHitSe = 0;
        }
    }
    if (s->wallHitWait != 0) s->wallHitWait--;
}
