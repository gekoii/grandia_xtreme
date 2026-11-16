/* Port-only C for GphGaObjDeformNearPoint (0x004393F8), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "protos/main.h"
#include "vu0_port.h"

f32 atan2f(f32, f32);

void VuVecCopy(f32 *dst, f32 *src);
f32 VuVecDot(f32 *a, f32 *b);
void VuVecNormalize(f32 *dst, f32 *src);
f32 VuVecNormalizeLen(f32 *dst, f32 *src);

/* The point: position, radius (+0x10), phase in degrees (+0x1C) */
typedef struct {
    /* 0x00 */ FVECTOR pos;
    /* 0x10 */ f32 radius;
    /* 0x14 */ u8 pad14[8];
    /* 0x1C */ f32 phase;
} NearPoint;

void GphGaObjDeformNearPoint(GphGaObject *obj, NearPoint *pt);

/*
 * Pulls the vertices near pt towards its height (r = pt->radius, d = the
 * vertex's horizontal distance): within 2r flattened onto it (halfway when
 * d > r), between 2r and 10r displaced by a sin/cos falloff with a phase
 * from the vertex's angle around pt. Writes the vertex packet and 4.12
 * normals after the normal packet's header.
 *
 * Kept as the asm has it: the "d.x < d.z^2 * 0.00625 - 2r" skip test, and
 * the e.x update that the following copy overwrites.
 *
 * The asm's centre is a stack quadword filled by VuVecCopy (xyz only); its
 * w, which the far branch stores into the packet, is stack garbage on the
 * PS2. The port stores the vertex's own w there.
 */
void GphGaObjDeformNearPoint(GphGaObject *obj, NearPoint *pt) {
    FVECTOR center;
    f32 *verts;
    f32 *pkt;
    s16 *nrm;
    f32 r;
    f32 r2;
    f32 phase;
    u32 n;
    u32 i;

    if (obj->vtxPkt == NULL || obj->deformBuf0 == NULL) {
        return;
    }
    obj->flags |= 2;
    center[3] = 0.0f;
    VuVecCopy(center, pt->pos);
    verts = (f32 *)obj->verts;
    r = pt->radius;
    n = obj->vtxCount;
    pkt = (f32 *)obj->vtxPkt;
    nrm = (s16 *)((u8 *)obj->deformBuf0 + 0x10);
    phase = pt->phase * VuBits(0x3C8EFA35); /* pi / 180 */
    if (n == 0) {
        return;
    }
    r2 = r + r;
    for (i = 0; i < n; i++) {
        FVECTOR p;
        FVECTOR d;
        FVECTOR e;
        f32 dist;
        f32 lim;
        f32 r10;
        s32 w[4];
        s32 c;

        VuCopy4(p, &verts[i * 4]);
        VuCopy4(d, p);
        for (c = 0; c < 3; c++) {
            d[c] = p[c] - center[c];
        }
        d[1] = 0.0f;
        dist = VuSqrt(VuVecDot(d, d));
        lim = d[2] * VuBits(0x3BCCCCCD) * d[2] - r2; /* 0.00625 */
        VuVecNormalize(e, d);
        if (d[0] < lim) {
            continue;
        }
        r10 = r * 10.0f;
        if (r10 < dist) {
            continue;
        }
        if (dist < r2) {
            /* Flattened: y = the point's, or halfway to it beyond r */
            VuCopy4(p, &verts[i * 4]);
            if (r < dist) {
                p[1] = (p[1] - center[1]) * 0.5f + center[1];
            } else {
                p[1] = center[1];
            }
            VuCopy4(&pkt[i * 4], p);
            d[0] = 0.0f;
            d[1] = 0.25f;
            d[2] = 0.0f;
            d[3] = 1.0f;
        } else {
            f32 a;
            f32 len;
            f32 s;
            f32 f;
            f32 *out = &pkt[i * 4];

            a = atan2f(e[2], e[0]) * 8.0f + (phase + phase);
            VuCopy4(p, out);
            if (dist < r * 5.0f) {
                VuCopy4(p, &verts[i * 4]);
            }
            VuCopy4(d, p);
            for (c = 0; c < 3; c++) {
                d[c] = p[c] - center[c];
            }
            VuVecCopy(e, d);
            e[1] = 0.0f;
            len = VuVecNormalizeLen(e, e);
            if (0.0f < d[0]) {
                e[0] = d[0] + len * e[0];
            }
            VuCopy4(e, d);
            e[1] = 0.0f;
            f = VuSqrt(VuVecDot(e, e)) / r10;
            if (1.0f < f) {
                f = 1.0f;
            }
            s = (sinf(a) + 1.0f) * 0.5f;
            s = s * sinf(f * 0.5f * VuBits(0x40490FD8)); /* pi */
            s = (1.0f - cosf(f * VuBits(0x3FC90FD8))) - s; /* pi / 2 */
            d[1] = d[1] * s;
            out[0] = center[0] + d[0];
            out[1] = center[1] + d[1];
            out[2] = center[2] + d[2];
            out[3] = d[3];
            d[0] = 0.0f;
            d[1] = s;
            d[2] = 0.0f;
            d[3] = 1.0f;
        }
        for (c = 0; c < 4; c++) {
            w[c] = VuFtoi(d[c], 12);
        }
        VuPackH4(&nrm[i * 4], w);
    }
}
