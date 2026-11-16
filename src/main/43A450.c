#include "common.h"
#include "main/n05.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

f32 GphShadowSetColor(FVECTOR arg0) {
    f32 f;
    copyq(gphShadowColor, arg0);
    f = arg0[3];
    gphShadowGsPkt.reg[1].addr = 0x42;
    gphShadowGsPkt.reg[1].data = ((u64)((s32)f & 0xFF) << 32) | 0x64;
    return f;
}
