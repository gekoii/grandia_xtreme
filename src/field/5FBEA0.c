/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/state.h"
#include "protos/field.h"


typedef struct {
    u8 pad0[0xC];
    FVECTOR *mtx;
} Node;

typedef struct {
    u8 pad0[0x24];
    s32 unk24;
} Mot;

typedef struct {
    u8 pad0[0xC];
    u16 unkC;
    u8 padE[0xE];
    u16 unk1C;
    u16 unk1E;
    u8 pad20[0x40];
    Mot *unk60;
} Obj;


void VuSinCosDeg(f32 *, f32 *, f32);

void ChrBreathPuffSpawn(Obj *o, s32 flag)
{
    FVECTOR v;
    f32 s;
    f32 c;
    Node *n;
    s32 i;

    if ((EvtGetMapNo() & 0xFF00) != 0x5000) {
        return;
    }
    if (EvtGetMapNo() == 0x5030) {
        return;
    }
    if (EvtGetMapNo() == 0x5024) {
        return;
    }
    if (EvtGetMapNo() == 0x50A0) {
        return;
    }
    if (EvtGetMapNo() == 0x50F0) {
        return;
    }
    if (o == NULL) {
        return;
    }
    if (!(o->unkC & 1)) {
        return;
    }
    if (flag) {
        n = ChrModelFindPart(o->unk60, 0x384);
    } else {
        n = ChrModelFindPart(o->unk60, 0x28A);
    }
    if (n == NULL) {
        return;
    }
    if (n->mtx == NULL) {
        return;
    }
    v[0] = 1.0f;
    v[1] = 1.0f;
    v[2] = 1.0f;
    v[3] = 1.0f;
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf4, 0x0(%0)\n\t"
        "lqc2 $vf5, 0x10(%0)\n\t"
        "lqc2 $vf6, 0x20(%0)\n\t"
        "lqc2 $vf7, 0x30(%0)\n\t"
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
        "sqc2 $vf1, 0(%1)"
        : : "r"(n->mtx), "r"(v) : "memory");
    if (FldVecDist(v, camField[0].lookAt) > 6000.0f) {
        return;
    }
    VuSinCosDeg(&s, &c, (f32)o->unk60->unk24 * 0.005493164f);
    v[0] += s * 20.0f;
    v[2] -= c * 20.0f;
    o->unk1C++;
    if (o->unk1C >= o->unk1E) {
        o->unk1C = 0;
    }
    if (o->unk1C != 0) {
        return;
    }
    o->unk1E = VuRandInt(60) + 30;
    if (VuRandInt(10) == 5) {
        for (i = 0; i < 8; i++) {
            v[0] += VuRandSigned(10.0f);
            v[1] += VuRandSigned(10.0f);
            v[2] += VuRandSigned(10.0f);
            FldSpriteSpawn(6, v, 0, 0x80, 1);
        }
    } else {
        FldSpriteSpawn(6, v, 0, 0x80, 1);
    }
}
