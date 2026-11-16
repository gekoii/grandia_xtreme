/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libdma.h"

extern u8 _sceDmaStsTable[];
extern u8 _sceDmaStdTable[];
extern u8 _sceDmaMfdTable[];
extern sceDmaEnv _sceDmaEnv;

s32 sceDmaPutEnv(sceDmaEnv *env) {
    u32 ctrl = *(volatile u32 *)0x1000E000;
    u32 pcr = *(volatile u32 *)0x1000E020;
    u32 sqwc = *(volatile u32 *)0x1000E030;
    void *rbor = *(void *volatile *)0x1000E050;
    u32 rbsr = *(volatile u32 *)0x1000E040;

    if (env->sts >= 10) {
        return -1;
    }
    if (env->std >= 10) {
        return -2;
    }
    if (env->mfd >= 10) {
        return -3;
    }
    if (env->rcycle >= 7) {
        return -4;
    }
    ctrl = (ctrl & 0xFFFFFFCF) | (_sceDmaStsTable[env->sts] << 4);
    ctrl = (ctrl & 0xFFFFFF3F) | (_sceDmaStdTable[env->std] << 6);
    ctrl = (ctrl & 0xFFFFFFF3) | (_sceDmaMfdTable[env->mfd] << 2);
    if (env->rcycle) {
        ctrl |= 2;
        ctrl = (ctrl & 0xFFFFFCFF) | ((env->rcycle - 1) << 8);
    } else {
        ctrl &= 0xFFFFFFFD;
    }
    pcr = (env->express << 16) | env->notify;
    sqwc = (env->tqwc << 16) | env->sqwc;
    rbor = env->rbadr;
    rbsr = env->rbmsk;
    *(volatile u32 *)0x1000E000 = ctrl;
    *(volatile u32 *)0x1000E020 = pcr;
    *(volatile u32 *)0x1000E030 = sqwc;
    *(void *volatile *)0x1000E050 = rbor;
    *(volatile u32 *)0x1000E040 = rbsr;
    _sceDmaEnv = *env;
    return 0;
}
