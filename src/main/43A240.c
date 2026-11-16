#include "common.h"
#include "protos/main.h"

#define UNIT_MATRIX(m) \
    __asm__ volatile( \
        "vmr32.xyzw $vf5, $vf0\n\t" \
        "vmr32.xyzw $vf6, $vf5\n\t" \
        "vmr32.xyzw $vf7, $vf6\n\t" \
        "sqc2 $vf0, 0x30(%0)\n\t" \
        "sqc2 $vf5, 0x20(%0)\n\t" \
        "sqc2 $vf6, 0x10(%0)\n\t" \
        "sqc2 $vf7, 0x0(%0)" \
        : : "r"(m) : "memory")

typedef struct {
    u8 pad0[2];
    u8 unk2;
    u8 pad3[0x49];
    s32 unk4C;
    u8 pad50[0x30];
} Sub;

typedef struct {
    u8 pad0[0xC];
    void *unkC;
    u8 pad10[0x30];
    Sub *subs;
    u8 pad44[0xC];
    u8 unk50;
    u8 pad51[0xD];
    u16 nsubs;
} Node;

typedef struct {
    u8 pad0[0x20];
    s32 unk20;
    s32 unk24;
} Stats;

void MemMove128(void *, void *, s32);
void GphMeshSkin(Node *, Sub *, void *);

void GphPartDrawCopy(Node *obj, s32 arg1) {
    s32 n;
    s32 i;
    s32 j;
    Sub *s;
    u8 k;
    s32 sz;

    n = obj->nsubs;
    if (n != 0) {
        if (obj->unk50 & 1) {
            sz = n << 7;
            s = (Sub *)GsPacketAlloc64(sz);
            MemMove128((void *)((u32)s | 0x20000000), obj->subs, (u32)sz >> 4);
            UNIT_MATRIX(obj->unkC);
        } else {
            s = obj->subs;
        }
        i = 0;
        j = 0;
        do {
            j++;
            i++;
            k = 7;
            if (*(s32 *)0x70002804 != 0) k = 0;
            s->unk2 = k;
            if (s->unk4C != 0) {
                GphMeshSkin(obj, s, obj->unkC);
            }
            GphDrawListAddGaObj(obj, s++, arg1);
            n--;
        } while (n > 0);
        ((Stats *)0x70002700)->unk24 += i;
        ((Stats *)0x70002700)->unk20 += j;
    }
}
