#include "common.h"
#include "ptcl.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}




typedef struct {
    u8 pad[0x5B0];
    s32 x, y, z;
} Rot;


void PtclFldEffectStart(u16 idx, f32 *pos, f32 *rot) {
    s32 i;
    GphModel *o;
    Rot *r;
    s32 off;
    s32 j;
    GphModel *o2;

    if (ptclFldWork->res[idx] != NULL) {
        ptclFldWork->active[idx] = 1;
        for (j = 0; j < 8; j++) {
            ptclFldWork->frame[idx][j] = 0;
        }
        off = idx << 4;
        r = (Rot *)((u8 *)ptclFldWork + off);
        r->x = rot[0] * 182.04445f;
        r->y = rot[1] * 182.04445f;
        r->z = rot[2] * 182.04445f;
        off += 0x4B0;
        copyq((u8 *)ptclFldWork + off, pos);
        for (i = 0; i < 8; i++) {
            o = ptclFldWork->models[idx][i];
            if (o != NULL) {
                copyq(o->pos, pos);
                o2 = ptclFldWork->models[idx][i];
                o2->rot[0] = rot[0] * 182.04445f;
                o2->rot[1] = rot[1] * 182.04445f;
                o2->rot[2] = rot[2] * 182.04445f;
            }
        }
    }
}
