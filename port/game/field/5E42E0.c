/* Port-only C for MapBorderSweepCircle (0x005E42E0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "vu.h"

/* The swept circle (the Mover of 0x005E3BC0/0x005E3F08) */
typedef struct MapBorderMover {
    /* 0x00 */ s32 hit;
    /* 0x04 */ u8 pad4[8];
    /* 0x0C */ f32 radius;
    /* 0x10 */ FVECTOR pos;
    /* 0x20 */ FVECTOR move;
    /* 0x30 */ FVECTOR stop;    /* move up to the contact */
    /* 0x40 */ FVECTOR slide;   /* the rest, along the obstacle */
} MapBorderMover;

f32 VuVecDot(f32 *, f32 *);
void VuVecCopy(f32 *, f32 *);
void VuVecNormalize(f32 *, f32 *);
s32 MapBorderSweepCircle(f32 *c, MapBorderMover *m, f32 radius);

/* v * k, xyz (vmulx.xyz) */
static void vscale(f32 *d, f32 *v, f32 k) {
    d[0] = v[0] * k;
    d[1] = v[1] * k;
    d[2] = v[2] * k;
}

/* Sweeps the mover's circle (in the XZ plane) against an upright
   cylinder of `radius` at c. On contact sets m->hit and fills stop and
   slide; returns 1 for a contact along the way, 0 when already touching
   (stop then just pushes out, slide is zero) or for no contact. */
s32 MapBorderSweepCircle(f32 *c, MapBorderMover *m, f32 radius) {
    FVECTOR up;
    FVECTOR d;       /* pos - c, horizontal */
    FVECTOR mv;
    FVECTOR vert;    /* the move's vertical part */
    FVECTOR hor;     /* its horizontal part */
    FVECTOR side;    /* across the horizontal move */
    FVECTOR end;     /* d + hor */
    FVECTOR t;
    FVECTOR u;
    FVECTOR s;
    f32 r = m->radius + radius;
    f32 horLen;
    f32 k;
    f32 off;         /* distance of c from the line of the move */
    f32 along;
    f32 back;
    f32 frac;

    up[0] = 0.0f;
    up[1] = 1.0f;
    up[2] = 0.0f;
    up[3] = 1.0f;
    d[0] = m->pos[0] - c[0];
    d[1] = m->pos[1] - c[1];
    d[2] = m->pos[2] - c[2];
    VuVecCopy(mv, m->move);
    __builtin_sqrtf(VuVecDot(mv, mv));
    vscale(vert, up, VuVecDot(mv, up));
    hor[0] = mv[0] - vert[0];
    hor[1] = mv[1] - vert[1];
    hor[2] = mv[2] - vert[2];
    horLen = __builtin_sqrtf(VuVecDot(hor, hor));
    vscale(t, up, VuVecDot(d, up));
    d[0] = d[0] - t[0];
    d[1] = d[1] - t[1];
    d[2] = d[2] - t[2];
    /* hor x up */
    side[0] = hor[1] * up[2] - up[1] * hor[2];
    side[1] = hor[2] * up[0] - up[2] * hor[0];
    side[2] = hor[0] * up[1] - up[0] * hor[1];
    VuVecNormalize(side, side);
    off = VuVecDot(side, d);
    end[0] = d[0] + hor[0];
    end[1] = d[1] + hor[1];
    end[2] = d[2] + hor[2];
    if (off < 0.0f) {
        vscale(side, side, -1.0f);
        off = -off;
    }
    if (r < off) {
        return 0;
    }
    if (0.0f < VuVecDot(hor, d)) {
        return 0;
    }
    if (VuVecDot(hor, end) < 0.0f && r < __builtin_sqrtf(VuVecDot(end, end))) {
        return 0;
    }
    VuVecDot(end, end);
    if (off < 0.01f) {
        /* already touching: push out along d */
        k = __builtin_sqrtf(VuVecDot(d, d)) - r;
        VuVecNormalize(t, d);
        vscale(t, t, -k);
        k = k / horLen;
        vscale(u, vert, k);
        u[0] = t[0] + u[0];
        u[1] = t[1] + u[1];
        u[2] = t[2] + u[2];
        VuVecCopy(m->stop, u);
        m->slide[0] = 0.0f;
        m->slide[1] = 0.0f;
        m->slide[2] = 0.0f;
        m->slide[3] = 1.0f;
        m->hit = 1;
        return 0;
    }
    /* sqrt.s takes |x|: level with the centre this rounds a little below 0,
       a NaN on the PC (the sweep then reported a hit with the whole move) */
    along = __builtin_sqrtf(__builtin_fabsf(VuVecDot(d, d) - off * off));
    vscale(s, side, off);
    k = r * r - off * off;
    back = 1.0f;
    if (1.0f < k) {
        back = __builtin_sqrtf(k);
    }
    k = along - back;
    if (k < 0.0f) {
        k = 0.0f;
    }
    VuVecNormalize(t, hor);
    vscale(u, t, k);
    frac = k / horLen;
    vscale(u, mv, frac);
    if (VuVecDot(mv, end) < 0.0f) {
        k = r / __builtin_sqrtf(VuVecDot(end, end));
        t[0] = d[0] + u[0];
        t[1] = d[1] + u[1];
        t[2] = d[2] + u[2];
        vscale(s, end, k);
        s[0] = s[0] - t[0];
        s[1] = s[1] - t[1];
        s[2] = s[2] - t[2];
    } else {
        vscale(s, side, r - off);
        s[0] = s[0] + mv[0];
        s[1] = s[1] + mv[1];
        s[2] = s[2] + mv[2];
        s[0] = s[0] - u[0];
        s[1] = s[1] - u[1];
        s[2] = s[2] - u[2];
        vscale(t, vert, 1.0f - frac);
        s[0] = s[0] + t[0];
        s[1] = s[1] + t[1];
        s[2] = s[2] + t[2];
    }
    VuVecCopy(m->stop, u);
    VuVecCopy(m->slide, s);
    m->hit = 1;
    return 1;
}
