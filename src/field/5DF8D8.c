#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


void EffSpawnCode35(f32 *arg0) {
    FVECTOR v;
    s32 i;

    copyq(v, arg0);
    v[1] += 120.0f;
    for (i = 0; i < 4; i++) {
        if (effMapWork->pickupPool[i]->active == 0) {
            PtclPresetRisingStreaks(effMapWork->pickupPool[i], v);
            return;
        }
    }
    PtclPresetRisingStreaks(effMapWork->pickupPool[0], v);
}
