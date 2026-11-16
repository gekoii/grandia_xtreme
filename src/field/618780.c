/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    u8 pad0[0x50];
    f32 pos[4];
} Cam;

extern Vec3 fldPointLightNearestPos;

void UtilSortU64(FldPointLightSortEnt *, u32, void *);
void MemMove32(s32, FldLightRec *, s32);

#define APPLY(out, m, v) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%2)\n\t" \
        "lqc2 $vf4, 0x0(%1)\n\t" \
        "lqc2 $vf5, 0x10(%1)\n\t" \
        "lqc2 $vf6, 0x20(%1)\n\t" \
        "lqc2 $vf7, 0x30(%1)\n\t" \
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t" \
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t" \
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t" \
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(out), "r"(m), "r"(v) : "memory")

void FldPointLightUpdate(Cam *cam)
{
    FVECTOR v;
    FldPointLight *e;
    FldLightRec *sub;
    GphModel *o;
    GphModel *p;
    ChrObj *o3;
    GphPart *n;
    s32 k;
    u32 i;

    for (i = 0; i < fldPointLightCount; i++) {
        fldPointLightSort[i].index = i;
        e = &fldPointLights[i];
        sub = &e->light;
        switch (e->kind) {
        case 1:
            o = (GphModel *)e->attach;
            if (o == NULL) {
                break;
            }
            if (e->unk6 != 0) {
                n = ChrModelFindPart(o, e->unk6);
                if (n == NULL) {
                    break;
                }
                v[0] = 1.0f;
                v[1] = 1.0f;
                v[2] = 1.0f;
                v[3] = 1.0f;
                APPLY(v, n->mtx, v);
                sub->pos[0] = v[0];
                sub->pos[1] = v[1];
                sub->pos[2] = v[2];
                sub->pos[3] = 1.0f;
            } else {
                sub->pos[0] = o->pos[0];
                sub->pos[1] = o->pos[1];
                sub->pos[2] = o->pos[2];
                sub->pos[3] = 1.0f;
            }
            break;
        case 2:
        case 3:
            o3 = (ChrObj *)e->attach;
            if (o3 == NULL) {
                break;
            }
            if (e->unk6 != 0) {
                n = ChrModelPartAt(o3->model, e->unk6);
                if (n == NULL) {
                    break;
                }
                v[0] = 1.0f;
                v[1] = 1.0f;
                v[2] = 1.0f;
                v[3] = 1.0f;
                APPLY(v, n->mtx, v);
                sub->pos[0] = v[0];
                sub->pos[1] = v[1];
                sub->pos[2] = v[2];
                sub->pos[3] = 1.0f;
            } else {
                p = o3->model;
                sub->pos[0] = p->pos[0];
                sub->pos[1] = p->pos[1];
                sub->pos[2] = p->pos[2];
                sub->pos[3] = 1.0f;
            }
            break;
        case 4: {
            FldPointLight *e4;
            v[0] = 1.0f;
            v[1] = 1.0f;
            v[2] = 1.0f;
            v[3] = 1.0f;
            APPLY(v, e->unk5C, v);
            sub->pos[0] = v[0];
            sub->pos[1] = v[1];
            sub->pos[2] = v[2];
            sub->pos[3] = 1.0f;
            /* indexed by hand: &fldPointLights[i] swaps the addu operands */
            e4 = (FldPointLight *)(i * sizeof(FldPointLight) + (u32)fldPointLights);
            e4->flickerTimer++;
            if (e4->flickerTimer >= 5) {
                f32 r = VuRandSigned(10.0f);
                FldPointLight *e5 = (FldPointLight *)(i * sizeof(FldPointLight) + (u32)fldPointLights);
                e5->flickerTimer = 0;
                sub->range = e5->unk18 + r;
            }
            break;
        }
        }
        fldPointLightNearestPos.x = sub->pos[0];
        fldPointLightNearestPos.y = sub->pos[1];
        fldPointLightNearestPos.z = sub->pos[2];
        fldPointLightSort[i].dist = FldVecDist(cam->pos, (f32 *)&fldPointLightNearestPos);
    }
    if (fldPointLightCount >= 5) {
        UtilSortU64(fldPointLightSort, fldPointLightCount, (void *)0x70003000);
    }
    k = 0;
    for (i = 0; i < fldPointLightCount; i++) {
        /* indexed by hand, as e4 above */
        FldPointLight *e2 = (FldPointLight *)(fldPointLightSort[i].index * sizeof(FldPointLight) + (u32)fldPointLights);
        if (e2->enable != 0) {
            if (k < 4) {
                if (e2->range > e2->unk18) {
                    e2->unk18 += 10.0f;
                }
                sub = (FldLightRec *)GphLightGet(k + 3);
                if (sub != NULL) {
                    MemMove32((s32)sub, &fldPointLights[fldPointLightSort[i].index].light, 0x10);
                }
            } else {
                e2->unk18 -= 10.0f;
                if (e2->unk18 < 0.0f) {
                    e2->unk18 = 0.0f;
                }
            }
            k++;
        }
    }
}
