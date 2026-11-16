/* Port-only C for GphMotKeyLerpQuat (0x00435468), which the decompilation still has in assembly. */
#include "common.h"
#include "vu0_port.h"

/* A quaternion key track */
typedef struct {
    /* 0x0 */ s32 count;      /* 1 = constant */
    /* 0x4 */ GrxQuad *quats; /* one per key */
    /* 0x8 */ u16 *keyOfFrame; /* frame -> key */
    /* 0xC */ u16 frames[1];  /* each key's frame */
} GphMotQuatTrack;

void GphMotQuatSlerp(f32 *out, f32 *a, f32 *b, f32 t);
void GphMotKeyLerpQuat(s32 track, f32 frame, void *out);

/* The rotation at frame: the key's quaternion if frame is on a key,
   otherwise GphMotQuatSlerp (a tail call) from the key at or before the
   frame to the next one. div.s: two keys on the same frame give a t of
   +-max, not Inf */
void GphMotKeyLerpQuat(s32 track, f32 frame, void *out) {
    GphMotQuatTrack *t = (GphMotQuatTrack *)track;
    u32 k;
    f32 f0;

    if (t->count == 1) {
        __builtin_memcpy(out, &t->quats[0], 16);
        return;
    }
    k = t->keyOfFrame[(s32)frame];
    f0 = (f32)(s32)t->frames[k];
    if (f0 == frame) {
        __builtin_memcpy(out, &t->quats[k], 16);
        return;
    }
    GphMotQuatSlerp(out, (f32 *)&t->quats[k], (f32 *)&t->quats[k + 1],
                    VuDiv(frame - f0, (f32)(s32)t->frames[k + 1] - f0));
}
