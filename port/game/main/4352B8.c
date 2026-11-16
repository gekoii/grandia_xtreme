/* Port-only C for GphMotKeyLerpVec (0x004352B8), which the decompilation still has in assembly. */
/*
 * Also GphMotKeyLerpRot (0x00435388).
 *
 * Argument order: the asm takes the keys in $a0, the output in $a1 and the
 * frame in $f12, so on the PS2 (keys, frame, out) and (keys, out, frame)
 * are the same call. On i686 they aren't. These take (keys, frame, out),
 * as GphMotKeyLerpQuat and every GphMotKeyLerpRot caller does; the
 * GphMotKeyLerpVec declarations in src/main/434FE8.c and 435028.c have
 * (keys, out, frame) and need swapping.
 */
#include "common.h"
#include "main/n04.h"
#include "vu0_port.h"

void GphMotKeyLerpVec(GphMotKeys *keys, f32 frame, void *out);
void GphMotKeyLerpRot(GphMotKeys *keys, f32 frame, void *out);

/* The key at or before frame (frameIndex is indexed by the truncated
 * frame); returns its index, and 1 when frame is exactly on it, else 0 and
 * the lerp weight towards the next key in *t. div.s: two keys on the same
 * frame give +-max, not Inf */
static s32 FindKey(GphMotKeys *keys, f32 frame, s32 *key, f32 *t) {
    s32 i = keys->frameIndex[(s32)frame];
    f32 f = (f32)(s32)keys->frames[i];

    *key = i;
    if (f == frame) {
        return 1;
    }
    *t = VuDiv(frame - f, (f32)(s32)keys->frames[i + 1] - f);
    return 0;
}

/* The VU's vmul/vmadd give +-max instead of Inf on overflow; the lerps
 * saturate each step so a +-max t (above) gives a finite value, not
 * Inf - Inf = NaN */
static f32 VuSat(f32 x) {
    return x > VU_FMAX ? VU_FMAX : x < -VU_FMAX ? -VU_FMAX : x;
}

/* Quadword keys (f32 x 4), lerped per component */
void GphMotKeyLerpVec(GphMotKeys *keys, f32 frame, void *out) {
    f32 *vals = keys->values;
    f32 *a;
    f32 *b;
    f32 r[4];
    f32 t;
    s32 i;
    s32 k;

    if (keys->count == 1) {
        VuCopy4(out, vals);
        return;
    }
    if (FindKey(keys, frame, &i, &t)) {
        VuCopy4(out, &vals[i * 4]);
        return;
    }
    a = &vals[i * 4];
    b = &vals[(i + 1) * 4];
    for (k = 0; k < 4; k++) {
        r[k] = VuSat(VuSat(b[k] * t) + VuSat(a[k] * (1.0f - t)));
    }
    VuCopy4(out, r);
}

/* s32 x 4 keys (16-bit angles): vitof0, lerp, vftoi0 */
void GphMotKeyLerpRot(GphMotKeys *keys, f32 frame, void *out) {
    s32 *vals = keys->values;
    s32 *a;
    s32 *b;
    s32 r[4];
    f32 t;
    s32 i;
    s32 k;

    if (keys->count == 1) {
        VuCopy4(out, vals);
        return;
    }
    if (FindKey(keys, frame, &i, &t)) {
        VuCopy4(out, &vals[i * 4]);
        return;
    }
    a = &vals[i * 4];
    b = &vals[(i + 1) * 4];
    for (k = 0; k < 4; k++) {
        r[k] = VuFtoi(VuSat(VuSat(VuItof(b[k], 0) * t) +
                            VuSat(VuItof(a[k], 0) * (1.0f - t))), 0);
    }
    VuCopy4(out, r);
}
