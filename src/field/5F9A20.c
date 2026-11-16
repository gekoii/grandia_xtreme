/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x10];
    FVECTOR pos;
    s32 x20;
    s32 x24;
} Mdl;

typedef struct {
    u8 pad0[0xC];
    u16 flags;
    u8 padE[0x54 - 0xE];
    s32 x54;
    u8 pad58[0x60 - 0x58];
    Mdl *mdl;
    u8 pad64[0x6C - 0x64];
    s32 x6C;
    u8 pad70[0x180 - 0x70];
    FVECTOR x180;
} Chara;

void ChrBreathPuffSpawn(Chara *, s32);
void ChrTrailGetDelayed(s32, f32 *);
void ChrObjMove(Chara *);
void ChrTrailPush(s32, f32 *, s32);

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void ChrFollowersUpdate(void) {
    FVECTOR v;
    FVECTOR a;
    Chara *pl;
    Chara *c;
    s32 i;

    pl = ChrGetByNo(1);
    for (i = 1; i < 9; i++) {
        c = ChrGetByNo(i + 1);
        if (c == NULL) {
            continue;
        }
        if (EvtSystemFlagGet(0x84) == 0 && EvtSystemFlagGet(i + 0x1F) == 1) {
            c->flags |= 1;
        }
        if (!(c->flags & 1)) {
            continue;
        }
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(c->x180) : "memory");
        ChrFindHeadPart(c);
        ChrTurnUpdate(c);
        ChrExtraPartsUpdate(c);
        ChrBreathPuffSpawn(c, 0);
        if (EvtSystemFlagGet(0x84) == 0 && EvtSystemFlagGet(i + 0x1F) == 1) {
            ChrTrailGetDelayed(pl->x6C, v);
            __asm__ volatile(
                "lqc2 $vf1, 0(%1)\n\t"
                "lqc2 $vf2, 0(%2)\n\t"
                "vsub.xyz $vf1, $vf1, $vf2\n\t"
                "sqc2 $vf1, 0(%0)"
                : : "r"(c->x180), "r"(v), "r"(c->mdl->pos) : "memory");
        }
        ChrObjMove(c);
        if (EvtSystemFlagGet(i + 0x1F) == 1) {
            if (fsqrt(c->x180[0] * c->x180[0] + c->x180[2] * c->x180[2]) > 0.01f) {
                ChrTrailPush(c->x6C, c->mdl->pos, 0);
            }
        }
        if (c->flags & 0x80) {
            if (fsqrt(c->x180[0] * c->x180[0] + c->x180[2] * c->x180[2]) > 0.01f) {
                FldDirToRot(c->x180, a);
                c->x54 = (u16)(s32)(a[1] * 10430.378f);
            }
        }
        c->mdl->x24 = c->x54 + 0x8000;
    }
}
