#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}


PtclEmitter *EffSpawnCode32(f32 *arg0, s32 arg1) {
    FVECTOR v;
    s32 i;

    copyq(v, arg0);
    for (i = 0; i < 4; i++) {
        if (effMapWork->spotPool[i]->active == 0) {
            PtclPresetPurpleWisps(effMapWork->spotPool[i], v, arg1);
            return effMapWork->spotPool[i];
        }
    }
    PtclPresetPurpleWisps(effMapWork->spotPool[0], v, arg1);
    return effMapWork->spotPool[0];
}
