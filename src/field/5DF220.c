#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"


s32 EffSpawnCode04(s32 arg0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (effMapWork->pool16[i]->active == 0) {
            PtclPresetSmoke(effMapWork->pool16[i], arg0);
            return (s32)effMapWork->pool16[i];
        }
    }
    PtclPresetSmoke(effMapWork->pool16[0], arg0);
    return (s32)effMapWork->pool16[0];
}

s32 EffSpawnCode4F(s32 arg0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (effMapWork->pool16[i]->active == 0) {
            PtclPresetSmokeLarge(effMapWork->pool16[i], arg0);
            return (s32)effMapWork->pool16[i];
        }
    }
    PtclPresetSmokeLarge(effMapWork->pool16[0], arg0);
    return (s32)effMapWork->pool16[0];
}
