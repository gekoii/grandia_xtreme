#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"


s32 EffSpawnCode5A(s32 arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (effMapWork->pool5A[i]->active == 0) {
            PtclPresetSparkleRise(effMapWork->pool5A[i], arg0);
            return (s32)effMapWork->pool5A[i];
        }
    }
    PtclPresetSparkleRise(effMapWork->pool5A[0], arg0);
    return (s32)effMapWork->pool5A[0];
}

s32 EffSpawnCode59(s32 arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (effMapWork->pool5A[i]->active == 0) {
            PtclPresetRedGlow(effMapWork->pool5A[i], arg0);
            return (s32)effMapWork->pool5A[i];
        }
    }
    PtclPresetRedGlow(effMapWork->pool5A[0], arg0);
    return (s32)effMapWork->pool5A[0];
}

s32 EffSpawnCode14(s32 arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (effMapWork->poolE0[i]->active == 0) {
            PtclPresetSparkFountain(effMapWork->poolE0[i], arg0);
            return (s32)effMapWork->poolE0[i];
        }
    }
    PtclPresetSparkFountain(effMapWork->poolE0[0], arg0);
    return (s32)effMapWork->poolE0[0];
}

s32 EffSpawnCode52(s32 arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (effMapWork->poolE0[i]->active == 0) {
            PtclPresetGoldSpray(effMapWork->poolE0[i], arg0);
            return (s32)effMapWork->poolE0[i];
        }
    }
    PtclPresetGoldSpray(effMapWork->poolE0[0], arg0);
    return (s32)effMapWork->poolE0[0];
}

s32 EffSpawnCode05(s32 arg0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (effMapWork->pool16[i]->active == 0) {
            PtclPresetSmokeSmall(effMapWork->pool16[i], arg0);
            return (s32)effMapWork->pool16[i];
        }
    }
    PtclPresetSmokeSmall(effMapWork->pool16[0], arg0);
    return (s32)effMapWork->pool16[0];
}

s32 EffSpawnUnusedA(s32 arg0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (effMapWork->pool16[i]->active == 0) {
            PtclPresetSmokeGrey(effMapWork->pool16[i], arg0);
            return (s32)effMapWork->pool16[i];
        }
    }
    PtclPresetSmokeGrey(effMapWork->pool16[0], arg0);
    return (s32)effMapWork->pool16[0];
}
