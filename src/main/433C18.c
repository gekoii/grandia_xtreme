#include "common.h"
#include "protos/main.h"

typedef struct {
    f32 pos[4];
    f32 nrm[4];
} Vtx;

typedef struct {
    u8 pad0[0x10];
    Vtx v[1];
} VtxBuf;

typedef struct {
    u8 pad0[0x6C];
    VtxBuf *unk6C;
    u8 pad70[0x30];
} Mdl;

typedef struct {
    u8 pad0[0x48];
    s32 unk48;
    u16 *unk4C;
    u8 pad50[0x10];
    u32 unk60;
    u32 unk64;
} Elem;

void MemMove128(void *, void *, s32);
void GphSkinPackNormals(void *, void *, s32);

Elem *GphMeshSkin(Mdl *m, Elem *e, f32 *mtx)
{
    s32 n;
    s32 size;
    s32 tag;
    s32 i;
    s32 cnt;
    u16 *p;
    f32 *o1;
    f32 *o2;
    f32 *base2;
    u16 *q;
    s16 b;
    Vtx *v;
    Mdl *md;
    Vtx *vb;
    s32 w;
    u32 buf1;
    u32 buf2;
    u32 *pk;

    n = e->unk48;
    p = e->unk4C;
    size = n << 4;
    tag = n << 16;
    base2 = (f32 *)(size + 0x70003000);
    o1 = (f32 *)0x70003000;
    o2 = base2;
    i = n;
    do {
        q = p + 1;
        cnt = *p;
        __asm__ volatile(
            "qmtc2.ni %z0, $vf3\n\t"
            "qmtc2.ni %z0, $vf4"
            : : "rJ"(0));
        for (; cnt > 0; cnt--) {
            b = q[1];
            w = q[0];
            md = &m[b];
            __asm__ volatile(
                "lqc2 $vf12, 0x0(%0)\n\t"
                "lqc2 $vf13, 0x10(%0)\n\t"
                "lqc2 $vf14, 0x20(%0)\n\t"
                "lqc2 $vf15, 0x30(%0)"
                : : "r"(mtx + b * 16) : "memory");
            vb = md->unk6C->v;
            v = &vb[q[2]];
            __asm__ volatile(
                "qmtc2.ni %1, $vf5\n\t"
                "lqc2 $vf1, 0x0(%0)\n\t"
                "vitof15.x $vf5, $vf5\n\t"
                "lqc2 $vf2, 0x10(%0)\n\t"
                "vmulax.xyzw ACC, $vf12, $vf1x\n\t"
                "vmadday.xyzw ACC, $vf13, $vf1y\n\t"
                "vmaddaz.xyzw ACC, $vf14, $vf1z\n\t"
                "vmaddw.xyzw $vf1, $vf15, $vf1w\n\t"
                "vmulax.xyz ACC, $vf12, $vf2x\n\t"
                "vmadday.xyz ACC, $vf13, $vf2y\n\t"
                "vmaddz.xyz $vf2, $vf14, $vf2z\n\t"
                "vmulax.xyz ACC, $vf1, $vf5x\n\t"
                "vmaddw.xyz $vf3, $vf3, $vf0w\n\t"
                "vmulax.xyz ACC, $vf2, $vf5x\n\t"
                "vmaddw.xyz $vf4, $vf4, $vf0w"
                : : "r"(v), "r"(w) : "memory");
            q += 3;
        }
        __asm__ volatile("sqc2 $vf3, 0(%0)" : : "r"(o1) : "memory");
        __asm__ volatile("sqc2 $vf4, 0(%0)" : : "r"(o2) : "memory");
        o1 += 4;
        o2 += 4;
        p = q;
    } while (--i != 0);
    size = n << 4;
    buf1 = GsPacketAlloc64(size);
    buf2 = GsPacketAlloc64(size + 0x10);
    MemMove128((void *)(buf1 | 0x20000000), (void *)0x70003000, n);
    pk = (u32 *)(buf2 | 0x20000000);
    pk[3] = tag | 0x6D000080;
    pk[1] = 0;
    pk[2] = 0;
    pk[0] = 0;
    GphSkinPackNormals((void *)((buf2 + 0x10) | 0x20000000), base2, n);
    e->unk60 = buf1;
    e->unk64 = buf2;
    return e;
}
