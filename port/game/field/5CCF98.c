/* Port-only C for CamFollowTrailMove (0x005CCF98), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "vu.h"
#include "field/n15.h"

extern s32 camFollowJustBlocked;
extern FVECTOR camTrailDir;

f32 VuVecDot(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
f32 VuVecNormalizeLen(f32 *, f32 *);
s32 CamFollowTrailMove(f32 *out, f32 *pos, f32 *ref);

/* out = the camera move this frame along the trail. The direction is the
   last trail segment (sp0 -> sp10) bent towards pos (perpendicular to the
   segment, in the plane through pos), or ref - pos when the segment is
   degenerate; past 1200 units of trail it heads straight for the trail's
   end instead. The asm leaves the speed's bits in v0. */
s32 CamFollowTrailMove(f32 *out, f32 *pos, f32 *ref) {
    FVECTOR prev;
    FVECTOR last;
    FVECTOR seg;
    FVECTOR to;
    FVECTOR side;
    FVECTOR bend;
    FVECTOR dir;
    f32 rate = camFollowCur->rate[1];
    f32 maxRate = rate * 10.0f;
    f32 trailLen;
    f32 dist;
    f32 spd;
    s32 n;
    s32 ret;

    n = CamTrailCount();
    CamTrailGet(n - 1, prev);
    CamTrailGet(n, last);
    seg[0] = prev[0] - last[0];
    seg[1] = prev[1] - last[1];
    seg[2] = prev[2] - last[2];
    seg[3] = prev[3];
    to[0] = last[0] - pos[0];
    to[1] = last[1] - pos[1];
    to[2] = last[2] - pos[2];
    to[3] = last[3];
    if (VuVecDot(seg, seg) < 0.001f) {
        bend[0] = ref[0] - pos[0];
        bend[1] = ref[1] - pos[1];
        bend[2] = ref[2] - pos[2];
        bend[3] = ref[3];
    } else {
        VuVecNormalize(seg, seg);
        VuVecNormalize(to, to);
        /* (seg x to) x to; w is seg's */
        side[0] = seg[1] * to[2] - to[1] * seg[2];
        side[1] = seg[2] * to[0] - to[2] * seg[0];
        side[2] = seg[0] * to[1] - to[0] * seg[1];
        side[3] = seg[3];
        bend[0] = side[1] * to[2] - to[1] * side[2];
        bend[1] = side[2] * to[0] - to[2] * side[0];
        bend[2] = side[0] * to[1] - to[0] * side[1];
        bend[3] = side[3];
    }
    VuVecNormalize(dir, bend);
    __builtin_memcpy(camTrailDir, dir, 16);
    trailLen = CamTrailLength();
    seg[0] = pos[0] - last[0];
    seg[1] = pos[1] - last[1];
    seg[2] = pos[2] - last[2];
    seg[3] = pos[3];
    to[0] = last[0] - prev[0];
    to[1] = last[1] - prev[1];
    to[2] = last[2] - prev[2];
    to[3] = last[3];
    VuVecNormalizeLen(seg, seg);
    VuVecNormalize(to, to);
    /* the angle is computed and dropped */
    acosf(__builtin_fabsf(VuVecDot(seg, to)));
    dist = VuVecNormalizeLen(seg, camFollowCur->unkA0);
    VuVecDot(dir, seg);
    spd = CamApproachSpeed(trailLen, dist, rate, maxRate);
    if (trailLen < 1200.0f) {
        if (camFollowJustBlocked != 0) {
            spd = rate;
        }
    } else {
        for (n = 0; n < 4; n++) {
            dir[n] = last[n] - pos[n];
        }
        VuVecNormalize(dir, dir);
    }
    dir[0] = dir[0] * spd;
    dir[1] = dir[1] * spd;
    dir[2] = dir[2] * spd;
    __builtin_memcpy(out, dir, 16);
    __builtin_memcpy(&ret, &spd, 4);
    return ret;
}
