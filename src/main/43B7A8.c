#include "common.h"

typedef struct {
    u8 pad[0x70];
    f32 pos[4];
    u8 pad2[0xA0 - 0x80];
} Obj;

typedef struct {
    u8 pad[0x40];
    u16 count;
    Obj *objs;
} Group;

void ChrModelAddColor(Group *g, f32 *v)
{
    s32 i;
    Obj *o = g->objs;

    for (i = 0; i < g->count; i++, o++) {
        __asm__ volatile(
            "lqc2 $vf1, 0(%0)\n\t"
            "lqc2 $vf2, 0(%1)\n\t"
            "vadd.xyz $vf1, $vf1, $vf2\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(o->pos), "r"(v) : "memory");
    }
}
