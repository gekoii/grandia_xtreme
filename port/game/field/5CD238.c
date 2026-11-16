/* Port-only C for CamRayTest (0x005CD238), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "vu.h"

/* The swept sphere FldCollSweep takes */
typedef struct CamRayHit {
    /* 0x00 */ s32 hit;
    /* 0x04 */ u8 pad4[8];
    /* 0x0C */ f32 radius;
    /* 0x10 */ FVECTOR pos;     /* start */
    /* 0x20 */ FVECTOR move;
    /* 0x30 */ FVECTOR stop;    /* move up to the first hit */
    /* 0x40 */ FVECTOR slide;   /* written by the map border sweep (MapBorderSweepWall,
                                   MapBorderSweepCircle); the game's frame had spare stack
                                   there, the port's frame has saved registers */
} CamRayHit;

f32 VuVecNormalizeLen(f32 *, f32 *);
s32 CamRayTest(f32 *ofs, f32 *base, f32 *from);

/* Sweeps a 10-unit sphere from `from` to base + ofs and pulls ofs in so
   that base + ofs stops one unit short of what it hits. Returns the hit
   flag. */
s32 CamRayTest(f32 *ofs, f32 *base, f32 *from) {
    FVECTOR v;
    CamRayHit h;
    f32 len;

    v[0] = ofs[0] + base[0] - from[0];
    v[1] = ofs[1] + base[1] - from[1];
    v[2] = ofs[2] + base[2] - from[2];
    v[3] = ofs[3];
    __builtin_memcpy(h.move, v, 16);
    __builtin_memcpy(h.stop, v, 16);
    __builtin_memcpy(h.pos, from, 16);
    h.radius = 10.0f;
    h.hit = 0;
    FldCollSweep((s32 *)&h);
    len = VuVecNormalizeLen(v, h.stop) - 1.0f;
    v[0] = v[0] * len + from[0] - base[0];
    v[1] = v[1] * len + from[1] - base[1];
    v[2] = v[2] * len + from[2] - base[2];
    /* the quadword store takes w from the normalised vector (h.stop.w) */
    __builtin_memcpy(ofs, v, 16);
    return h.hit;
}
