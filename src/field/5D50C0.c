/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "sys.h"

#include "vu.h"
#include "field/state.h"

typedef struct {
    u8 pad0[0x10];
    f32 x;
    f32 y;
    f32 z;
} Mdl;

typedef struct {
    u8 pad0[0x60];
    Mdl *mdl;
    s32 unk64;
} Obj;

typedef struct {
    u8 pad0[8];
    s16 unk8;
    u8 padA[0x16];
    s32 unk20;
} Snd;


void FldTreasureSpawnDrops(void *);
void VuBezier3(FVECTOR *, FVECTOR *, f32);
void VuVecLerp(FVECTOR *, FVECTOR *, FVECTOR *, f32);
void GphQuad3DSetCorners(GphQuad3D *, FVECTOR *, FVECTOR *, f32, f32);
void VuProjectVerts(FVECTOR *, void *, FVECTOR *, s32);

#define E (((FldTreasureWork *)fldTreasureWork)->entries[i])
#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

void FldTreasureUpdate(void) {
    FVECTOR a;
    FVECTOR mtx[4];
    FVECTOR v0;
    FVECTOR v1;
    GphQuad3D q;
    s32 buf[8];
    FVECTOR scr[4];
    FVECTOR mid;
    FVECTOR d;
    Obj *pl;
    Snd *s;
    s32 i;
    s32 prev;
    s32 cnt;
    s32 h;
    s32 done;
    f32 t;
    f32 f;
    f32 g;

    prev = 0;
    pl = ChrGetByNo(1);
    VuMtxSetRotXY(mtx, camField[0].pitch, camField[0].yaw, 0.0f);
    if (btlHandoff.dropCount != 0) {
        FldTreasureSpawnDrops(btlHandoff.pos);
        btlHandoff.dropCount = 0;
        btlHandoff.unkC = 0;
    }
    cnt = 0;
    for (i = 0; i < 64; i++) {
        switch (E.state) {
        case 1:
            E.t += E.fallStep;
            if (E.t > 1.0f) {
                E.t = 1.0f;
                if (E.unk86) {
                    E.state = 3;
                } else {
                    E.state = 2;
                }
            }
            VuBezier3(&a, E.path, E.t);
            ChrInstSetPos(E.instGroup, E.inst, &a);
            if (E.unk86) {
                ChrInstSetPos(0x4D, E.inst2, &a);
            }
            break;
        case 2:
            E.dist = FldVecDist(&pl->mdl->x, E.path[3]);
            if (FldInputLockGet() == 1) {
                E.dist = 270.0f;
            }
            if (E.dist < 260.0f) {
                E.t += 0.1f;
                if (E.t > 3.0f) {
                    E.t = 3.0f;
                }
            } else {
                E.t -= 0.1f;
                if (E.t < 0.0f) {
                    E.t = 0.0f;
                }
            }
            if (camCtl.aiType == 2 || pl->unk64 != 0) {
                E.t = 0.0f;
            }
            f = E.t;
            t = f;
            if (f > 1.0f) {
                f = 1.0f;
            }
            if (t < 0.1f) {
                break;
            }
            ItemInfoFill(buf, E.item, 0);
            E.nameLen = TxtGlyphQueueStr(2, buf[3], prev);
            E.nameRow = prev;
            v0[0] = 0.0f;
            prev = cnt + 1;
            v0[1] = 0.0f;
            v0[2] = E.path[3][1] + 250.0f;
            v0[3] = 0.0f;
            v1[0] = TxtStrGetWidth(buf[3], 12);
            v1[1] = 12.0f;
            v1[2] = E.path[3][1] + 200.0f;
            v1[3] = 0.0f;
            VuVecLerp(&a, &v0, &v1, f);
            E.path[2][1] = a[2];
            GphQuad3DSetCorners(&q, mtx, &E.path[2], a[0], a[1]);
            VuProjectVerts(scr, VuMtxStackGetTop(), (FVECTOR *)q.pos, 4);
            VuVecLerp(&mid, &scr[0], &scr[3], 0.5f);
            __asm__ volatile(
                "lqc2 $vf1, 0(%0)\n\t"
                "lqc2 $vf2, 0(%1)\n\t"
                "vsub.xyz $vf1, $vf1, $vf2\n\t"
                "sqc2 $vf1, 0(%2)"
                : : "r"(scr[3]), "r"(scr[0]), "r"(d) : "memory");
            d[0] *= 0.5f;
            d[1] *= 0.5f;
            copyq(E.namePos, mid);
            cnt++;
            E.nameW = d[0];
            E.nameH = d[1];
            break;
        case 4:
            s = ChrInstGetPart(0x4D, E.inst2, 2);
            if (s == NULL) {
                break;
            }
            s->unk8 = 0;
            if (s->unk20 == 0) {
                s->unk20 = 0x10;
            }
            s->unk20 += s->unk20 / 10;
            if (s->unk20 >= 0x1000) {
                if (s->unk20 < 0x1200) {
                    EffSpawnPickup(&E.path[3]);
                }
            }
            if (s->unk20 > 0x3000) {
                E.state = 5;
                                ChrInstSetVisible(E.instGroup, E.inst, 1);
                                ChrModelSetAlphaRaw(ChrInstGetModel(E.instGroup, E.inst), -127.0f);
            }
            break;
        case 5:
            done = 0;
            h = ChrInstGetModel(0x4D, E.inst2);
            g = ChrModelGetAlphaRaw(h);
            if (g > -127.0f) {
                done = 1;
                ChrModelSetAlphaRaw(h, g - 8.0f);
            }
            if (done) {
                break;
            }
            h = ChrInstGetModel(E.instGroup, E.inst);
            g = ChrModelGetAlphaRaw(h);
            if (g < 0.0f) {
                g += 8.0f;
                if (g > 0.0f) {
                    g = 0.0f;
                }
                ChrModelSetAlphaRaw(h, g);
                done = 1;
            }
            if (done) {
                break;
            }
            SndSePlay(0x7E9, 0, 0, 10, 0);
            ChrInstSetVisible(0x4D, E.inst2, 0);
            E.state = 2;
            break;
        case 0:
        case 6:
            break;
        }
    }
}
