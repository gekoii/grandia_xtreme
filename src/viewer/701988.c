#include "common.h"
#include "viewer/n35.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void VwrFlockSetWaypoint(u8 idx, VuVec4 *src) {
    copyq(&vwrFlockWaypoints.v[idx], src);
}
