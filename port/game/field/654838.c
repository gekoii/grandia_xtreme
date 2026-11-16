/* Port-only C for WmapVecApproach (0x00654838), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "vu.h"

f32 VuVecDot(f32 *, f32 *);
void VuVecCopy(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
f32 VuVecNormalizeLen(f32 *, f32 *);
void WmapVecApproach(f32 *vel, f32 *pos, f32 *goal, f32 maxSpeed, f32 accel);

/* Moves pos towards goal with velocity vel: the part of vel along the way
   speeds up by accel up to maxSpeed and brakes in time to stop, the part
   across it dies down by accel a frame. Snaps to goal within accel; moves
   straight at accel within 3 * accel. */
void WmapVecApproach(f32 *vel, f32 *pos, f32 *goal, f32 maxSpeed, f32 accel) {
    FVECTOR to;
    FVECTOR dir;
    FVECTOR along;
    FVECTOR across;
    FVECTOR t;
    FVECTOR step;
    f32 k;
    f32 dist;
    f32 spd;
    f32 rest;
    s32 i;

    to[0] = goal[0] - pos[0];
    to[1] = goal[1] - pos[1];
    to[2] = goal[2] - pos[2];
    to[3] = goal[3];
    VuVecNormalize(dir, to);
    k = VuVecDot(dir, vel);
    for (i = 0; i < 4; i++) {
        along[i] = dir[i] * k;
    }
    for (i = 0; i < 4; i++) {
        across[i] = vel[i] - along[i];
    }
    dist = __builtin_sqrtf(VuVecDot(to, to));
    if (dist <= accel) {
        __builtin_memcpy(pos, goal, 16);
        vel[0] = 0.0f;
        vel[1] = 0.0f;
        vel[2] = 0.0f;
        vel[3] = 1.0f;
        return;
    }
    if (dist < accel * 3.0f) {
        VuVecNormalizeLen(to, to);
        t[0] = to[0] * accel;
        t[1] = to[1] * accel;
        t[2] = to[2] * accel;
        t[3] = to[3];
        for (i = 0; i < 4; i++) {
            pos[i] = pos[i] + t[i];
        }
        __builtin_memcpy(vel, t, 16);
        return;
    }
    /* along the way */
    t[0] = along[0] + dir[0] * accel;
    t[1] = along[1] + dir[1] * accel;
    t[2] = along[2] + dir[2] * accel;
    t[3] = along[3];
    if (0.0f < VuVecDot(dir, t)) {
        spd = __builtin_sqrtf(VuVecDot(t, t));
        t[0] = to[0] - t[0];
        t[1] = to[1] - t[1];
        t[2] = to[2] - t[2];
        t[3] = to[3];
        rest = VuVecNormalizeLen(t, t);
        if (maxSpeed < spd) {
            spd = maxSpeed;
        }
        if (rest < 1.0f / accel * spd * spd) {
            spd = spd - (accel + accel);
        }
        along[0] = dir[0] * spd;
        along[1] = dir[1] * spd;
        along[2] = dir[2] * spd;
        along[3] = dir[3];
    } else {
        VuVecCopy(along, t);
    }
    /* across it */
    if (0.0f < __builtin_sqrtf(VuVecDot(across, across))) {
        VuVecNormalize(t, across);
        t[0] = across[0] - t[0] * accel;
        t[1] = across[1] - t[1] * accel;
        t[2] = across[2] - t[2] * accel;
        t[3] = across[3];
        if (VuVecDot(t, across) < 0.0f) {
            across[0] = 0.0f;
            across[1] = 0.0f;
            across[2] = 0.0f;
            across[3] = 1.0f;
        } else {
            VuVecCopy(across, t);
        }
    }
    step[0] = along[0] + across[0];
    step[1] = along[1] + across[1];
    step[2] = along[2] + across[2];
    step[3] = along[3];
    pos[0] = pos[0] + step[0];
    pos[1] = pos[1] + step[1];
    pos[2] = pos[2] + step[2];
    VuVecCopy(vel, step);
}
