#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n28.h"

#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

void VuSinCosAngle(f32 *, f32 *, s32);

void PtclBtlDustSpawn(s32 arg0, BtlUnit *arg1, f32 x, f32 z) {
    f32 a0, a1;
    f32 b0, b1;
    f32 c0, c1;
    PtclBtlDust *p;
    PtclBtlDust *q;
    PtclBtlDust *r;
    PtclBtlDust *t;
    s32 k;
    PtclBtlDustRing *e;
    PtclBtlDustSpray *f;
    s32 idx;
    s32 j;

    switch (arg0) {
    case 1:
        p = PtclBtlDustAlloc();
        if (p != NULL) {
            p->unkC = arg0;
            p->unk4 = 40.0f;
            p->unk8 = 0;
            p->unk0 = 1.0f;
            p->unkE = rand() >> 15;
            p->unk10 = x + ((f32)(rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 20.0f;
            p->unk14 = 40.0f;
            p->unk18 = z + ((f32)(rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 20.0f;
            p->unk24 = 2.0f * ((f32)(rand() >> 15) * (1.0f / 65536.0f));
            VuSinCosAngle(&a0, &a1, rand() >> 15);
            p->unk20 = 2.0f * a0 * ((f32)(rand() >> 15) * (1.0f / 65536.0f));
            p->unk28 = 2.0f * a1 * ((f32)(rand() >> 15) * (1.0f / 65536.0f));
        }
        break;
    case 2:
        q = PtclBtlDustAlloc();
        if (q != NULL) {
            q->unkC = arg0;
            q->unk8 = 0;
            q->unk0 = 1.0f;
            q->unk4 = 100.0f;
            q->unk10 = x + ((f32)(rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 20.0f;
            q->unk14 = 80.0f;
            q->unk18 = z + ((f32)(rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 20.0f;
        }
        break;
    case 0x80:
    case 0x81:
    case 0x82:
        idx = PtclBtlDustGroupFindFree();
        if (idx < 32) {
            e = BtlAlloc32(0x1A0);
            ptclBtlDustGroups[idx] = e;
            if (e != NULL) {
                e->timer = 0;
                e->kind = 0x80;
                e->x = x + ((f32)(rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 20.0f;
                e->y = 40.0f;
                e->z = z + ((f32)(rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 20.0f;
                switch (arg0) {
                default:
                    e->spread = 0.0f;
                    break;
                case 0x81:
                    e->spread = 0.3f;
                    break;
                case 0x82:
                    e->spread = 0.6f;
                    break;
                }
                for (k = 0; k < 32; k++) {
                    if (k >= 16 && (arg0 == 0x80 || (k >= 24 && arg0 == 0x81))) {
                        break;
                    }
                    r = PtclBtlDustAlloc();
                    e->sprites[k] = (PtclBtlDust *)r;
                    if (r == NULL) {
                        break;
                    }
                    r->unkC = 0;
                    r->unk0 = 1.0f;
                    copyq(&r->unk10, &e->x);
                    VuSinCosAngle(&b0, &b1, rand() >> 15);
                    switch (arg0) {
                    default:
                        r->unk4 = 40.0f;
                        e->vx[k] = b0 * 128.0f;
                        e->vz[k] = b1 * 128.0f;
                        break;
                    case 0x81:
                        r->unk4 = 70.0f;
                        e->vx[k] = b0 * 128.0f * 1.5f;
                        e->vz[k] = b1 * 128.0f * 1.5f;
                        break;
                    case 0x82:
                        r->unk4 = 100.0f;
                        e->vx[k] = b0 * 128.0f * 2.0f;
                        e->vz[k] = b1 * 128.0f * 2.0f;
                        break;
                    }
                }
                for (; k < 32; k++) {
                    e->sprites[k] = NULL;
                }
            }
        }
        break;
    case 0x83:
        idx = PtclBtlDustGroupFindFree();
        if (idx < 32) {
            f = BtlAlloc32(0x40);
            ptclBtlDustGroups[idx] = f;
            if (f != NULL) {
                f->kind = arg0;
                f->timer = 0;
                VuSinCosAngle(&c0, &c1, arg1->rot[1]);
                f->x = x + c0 * 50.0f + ((f32)(rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 20.0f;
                f->y = 40.0f;
                f->z = z + c1 * 50.0f + ((f32)(rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 20.0f;
                VuSinCosAngle(&c0, &c1, arg1->rot[1] - 0x6000);
                f->vx[0] = c0 * 128.0f;
                f->vz[0] = c1 * 128.0f;
                VuSinCosAngle(&c0, &c1, arg1->rot[1] + 0x6000);
                f->vx[1] = c0 * 128.0f;
                f->vz[1] = c1 * 128.0f;
                for (j = 0; j < 4; j++) {
                    t = PtclBtlDustAlloc();
                    f->sprites[j] = (PtclBtlDust *)t;
                    if (t == NULL) {
                        break;
                    }
                    t->unk0 = 1.0f;
                    if (j & 1) {
                        t->unk4 = 60.0f;
                    } else {
                        t->unk4 = 40.0f;
                    }
                    t->unkC = 0;
                    copyq(&t->unk10, &f->x);
                }
                for (; j < 4; j++) {
                    f->sprites[j] = NULL;
                }
            }
        }
        break;
    }
}
