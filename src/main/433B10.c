#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 pad0[0x4C];
    s32 unk4C;
    u8 pad50[0x30];
} Elem;

typedef struct {
    u8 pad0[0xC];
    f32 *unkC;
    u8 pad10[0x30];
    void *unk40;
    Elem *unk44;
    u8 pad48[8];
    u8 unk50;
    u8 pad51[0xD];
    u16 unk5E;
    u8 pad60[0x40];
} Mdl;

void MemMove128(void *, void *, u32);
void GphMeshSkin(Mdl *, Elem *, f32 *);

void GphModelSkin(Mdl *m, s32 n)
{
    s32 cnt;
    u32 size;
    Elem *e;

    for (; n > 0; m++, n--) {
        if (m->unk50 & 1) {
            cnt = m->unk5E;
            size = cnt << 7;
            e = (Elem *)GsPacketAlloc64(size);
            MemMove128((void *)((u32)e | 0x20000000), m->unk40, size >> 4);
            m->unk44 = e;
            for (; cnt > 0; cnt--, e++) {
                if (e->unk4C) {
                    GphMeshSkin(m, e, m->unkC);
                }
            }
            __asm__ volatile(
                "vmr32.xyzw $vf5, $vf0\n\t"
                "vmr32.xyzw $vf6, $vf5\n\t"
                "vmr32.xyzw $vf7, $vf6\n\t"
                "sqc2 $vf0, 0x30(%0)\n\t"
                "sqc2 $vf5, 0x20(%0)\n\t"
                "sqc2 $vf6, 0x10(%0)\n\t"
                "sqc2 $vf7, 0x0(%0)"
                : : "r"(m->unkC) : "memory");
        }
    }
}
