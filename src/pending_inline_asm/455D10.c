#include "common.h"

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) Vec4;

typedef struct {
    u8 pad[0x80];
    Vec4 v;
    f32 sx, sy, sz;
} Obj;

typedef struct {
    Vec4 v;
    u8 pad[0x40];
} Local;

void func_004558C0(s32, Local *);

void func_00455D10(s32 arg0, Obj *arg1) {
    Local l;
    f32 m;

    m = arg1->sx;
    if (m < arg1->sy) {
        m = arg1->sy;
    }
    if (m < arg1->sz) {
        m = arg1->sz;
    }
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(&l.v), "r"(&arg1->v) : "$2", "memory");
    l.v.w = m;
    func_004558C0(arg0, &l);
}
