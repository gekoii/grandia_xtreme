#include "common.h"
#include "protos/main.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    FVECTOR *unkC;
    u8 pad10[0x38];
    s32 unk48;
    u8 pad4C[0x80 - 0x4C];
} Elem;

typedef struct {
    u8 pad0[0x40];
    Elem *unk40;
    u8 pad44[0x1A];
    u16 unk5E;
} Obj;


#define vscale(dst, src, k) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "qmtc2.ni %2, $vf2\n\t" \
        "vmulx.xyzw $vf1, $vf1, $vf2x\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(src), "r"(k) : "memory")

#define vadd(dst, a, b) \
    __asm__ volatile( \
        "lqc2 $vf1, 0(%1)\n\t" \
        "lqc2 $vf2, 0(%2)\n\t" \
        "vadd.xyzw $vf1, $vf1, $vf2\n\t" \
        "sqc2 $vf1, 0(%0)" \
        : : "r"(dst), "r"(a), "r"(b) : "memory")

void GphMorphPositionsLerp(Obj *o, FVECTOR *a, FVECTOR *b, f32 t)
{
    FVECTOR t0;
    FVECTOR t1;
    s32 i;
    s32 j;
    s32 n;
    Elem *e;
    FVECTOR *d;
    f32 s;
    f32 u;
    u16 cnt;

    s = 1.0f - t;
    u = t;
    cnt = o->unk5E;
    e = o->unk40;
    for (i = 0; i < cnt; e++, i++) {
        n = e->unk48;
        d = GsPacketAlloc64(n * 16);
        e->unkC = d;
        e->unk1 |= 2;
        for (j = 0; j < n; j++) {
            vscale(t0, a, s);
            vscale(t1, b, u);
            vadd(d, t0, t1);
            d++;
            a++;
            b++;
        }
    }
}
