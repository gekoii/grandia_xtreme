/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"


void BtlVecRandomDir(f32 *);

#define VADD_XYZ(d, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyz $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(d), "r"(a), "r"(b) : "memory")

void PtclBurstUpdateSpark(PtclBurst *e) {
    f32 tmp[4] __attribute__((aligned(16)));
    PtclBtlSpr *p;
    PtclBtlSpr *q;
    PtclBtlSpr *p2;
    s32 n;
    s32 i;

    if (e->active > 0) {
        if (e->unk8 != btlFrameCount) {
            PtclBurstFree(e);
        }
        return;
    }
    if (e->count < e->max) {
        e->timer += e->rate;
        while (e->timer >= 1.0f) {
            if (e->count < e->max) {
                e->active = 0;
                VADD_XYZ(tmp, ((BtlUnit *)e->unk3C)->pos, e->ofs);
                p = &((PtclBtlSpr *)e->parts)[e->count];
                BtlVecRandomDir(p->pos);
                p->flags = (rand() >> 15) & 3;
                p->lifeExtra = (rand() >> 15) * (1.0f / 65536.0f) * ptclSparkKeys.lifeRand;
                p->life = ptclSparkKeys.life + p->lifeExtra + 1;
                if (e->type == 13 || e->type == 17 || e->type == 21) {
                    PtclBtlSprSetSpeed(p, 1.0f, 8.0f);
                    p->scale = ((rand() >> 15) * (1.0f / 65536.0f) * 10.0f + 10.0f) * 1.3f;
                } else if (e->type == 14 || e->type == 18 || e->type == 22) {
                    PtclBtlSprSetSpeed(p, 1.0f, 6.0f);
                    p->scale = (rand() >> 15) * (1.0f / 65536.0f) * 8.0f + 8.0f;
                } else {
                    PtclBtlSprSetSpeed(p, 1.0f, 6.0f);
                    p->scale = (rand() >> 15) * (1.0f / 65536.0f) * 8.0f + 6.0f;
                }
                VADD_XYZ(p->pos, p->pos, tmp);
                e->count++;
            }
            e->timer -= 1.0f;
        }
    }
    if (e->parts2 != NULL && e->count2 < e->max2) {
        e->timer2 += e->rate2;
        while (e->timer2 >= 1.0f) {
            if (e->count2 < e->max2) {
                VADD_XYZ(tmp, ((BtlUnit *)e->unk3C)->pos, e->ofs);
                p2 = &e->parts2[e->count2];
                BtlVecRandomDir(p2->pos);
                p2->flags = (rand() >> 15) & 3;
                p2->lifeExtra = (rand() >> 15) * (1.0f / 65536.0f) * ptclSparkGlowKeys.lifeRand;
                p2->life = ptclSparkGlowKeys.life + p2->lifeExtra + 1;
                if (e->type == 13 || e->type == 17 || e->type == 21) {
                    PtclBtlSprSetSpeed(p2, 3.0f, 10.0f);
                    p2->scale = ((rand() >> 15) * (1.0f / 65536.0f) * 15.0f + 8.0f) * 1.3f;
                } else if (e->type == 14 || e->type == 18 || e->type == 22) {
                    PtclBtlSprSetSpeed(p2, 3.0f, 8.0f);
                    p2->scale = (rand() >> 15) * (1.0f / 65536.0f) * 10.0f + 6.0f;
                } else {
                    PtclBtlSprSetSpeed(p2, 3.0f, 6.0f);
                    p2->scale = (rand() >> 15) * (1.0f / 65536.0f) * 8.0f + 4.0f;
                }
                VADD_XYZ(p2->pos, p2->pos, tmp);
                e->count2++;
            }
            e->timer2 -= 1.0f;
        }
    }
    n = 0;
    q = e->parts;
    for (i = e->max; i > 0; i--, q++) {
        if (q->life > 0) {
            q->life--;
            q->pos[0] += q->vel[0];
            q->pos[1] += q->vel[1];
            q->pos[2] += q->vel[2];
            q->vel[0] += q->accel[0];
            q->vel[1] += q->accel[1] - 0.07f;
            q->vel[2] += q->accel[2];
            n++;
        }
    }
    if (e->parts2 != NULL) {
        q = e->parts2;
        for (i = e->max2; i > 0; i--, q++) {
            if (q->life > 0) {
                q->life--;
                q->pos[0] += q->vel[0];
                q->pos[1] += q->vel[1];
                q->pos[2] += q->vel[2];
                q->vel[0] += q->accel[0];
                q->vel[1] += q->accel[1];
                q->vel[2] += q->accel[2];
                n++;
            }
        }
    }
    if (n <= 0 && e->active == 0) {
        e->active = 1;
        e->unk8 = btlFrameCount;
    }
}
