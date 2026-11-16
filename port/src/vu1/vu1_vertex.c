/* The VU1 program's vertex stages: header, colour conversion, position and
   normal transforms, lighting and projection. See vu1_internal.h and
   port/docs/vu.md. */
#include "vu1_internal.h"

#pragma STDC FP_CONTRACT OFF

/* The used-vertex list of the VU0 paths: the w words of the position slots
   [first, count) hold the vertex indices (GphVertCompact, GphVertIndex) */
static uint32_t list_entry(uint32_t slot) {
    return vu1_ilw(VU1_POS + slot, W);
}

static float maxf(float a, float b) {
    return a > b ? a : b;
}

static float minf(float a, float b) {
    return a < b ? a : b;
}

/* ---------------------------------------------------------------------- */
/* Header (0x0A78, 0x0B30, 0x0BC0, 0x0CB0) */

void vu1_load_header(int layout) {
    V4 *vf = grx_vu1.vf;
    int i;

    for (i = 0; i < 5; i++) {
        vf[VF_PROJ + i] = vu1_ld(VU1_HEADER + i);
    }
    if (layout == HDR_VU0_LIT || layout == HDR_VU1_LIT) {
        for (i = 0; i < 3; i++) {
            vf[VF_LMTX + i] = vu1_ld(VU1_HEADER + 5 + i);
        }
    }
    if (layout == HDR_VU0_UNLIT || layout == HDR_VU0_LIT) {
        vf[VF_FOG] = vu1_ld(VU1_HEADER + 8);
        vf[VF_UVOFF] = vu1_ld(VU1_HEADER + 9);
    } else {
        for (i = 0; i < 4; i++) {
            vf[VF_MV + i] = vu1_ld(VU1_HEADER + 8 + i);
        }
        vf[VF_FOG] = vu1_ld(VU1_HEADER + 12);
        vf[VF_UVOFF] = vu1_ld(VU1_HEADER + 13);
    }
}

/* The VU0 paths pad the list with 7 entries of the dummy vertex 127, so
   that the pipelined loops' read-ahead stays in bounds (0x0AB8, 0x0C18) */
void vu1_pad_list(void) {
    uint32_t count = vu1_count();
    int k;

    for (k = 0; k < 7; k++) {
        vu1_isw(VU1_POS + count + k, W, VU1_PAD_INDEX);
    }
}

/* ---------------------------------------------------------------------- */
/* Colours to float: itof0 of n colour quadwords (sub_0178, sub_0268) */

void vu1_colours_to_float(uint32_t first_qw, uint32_t n) {
    uint32_t q;
    int i;

    for (q = first_qw; q < first_qw + n; q++) {
        V4 c = vu1_ld(q), r;
        for (i = 0; i < 4; i++) {
            r.f[i] = vu_itof(c.i[i], 0);
        }
        vu1_st(q, &r, MXYZW);
    }
}

/* ---------------------------------------------------------------------- */
/* Positions: model space to view space by the model-view matrix vf24-vf27,
   xyz written back, w (the vertex index) kept (sub_0268, sub_0420) */

void vu1_transform_positions(uint32_t n) {
    const V4 *m = &grx_vu1.vf[VF_MV];
    uint32_t v;
    int i;

    for (v = 0; v < n; v++) {
        V4 p = vu1_ld(VU1_POS + v), r;
        for (i = 0; i < 4; i++) {
            float acc = m[0].f[i] * p.f[X];  /* mulax */
            acc = acc + m[1].f[i] * p.f[Y]; /* madday */
            acc = acc + m[2].f[i] * p.f[Z]; /* maddaz */
            r.f[i] = acc + m[3].f[i] * 1.0f; /* maddw by vf00w */
        }
        vu1_st(VU1_POS + v, &r, MXYZ);
    }
}

/* ---------------------------------------------------------------------- */
/* Normals: 4.12 fixed to float, times the light matrix vf21-vf23, xyz
   written back */

static void normal_result(V4 *r, uint32_t slot) {
    const V4 *l = &grx_vu1.vf[VF_LMTX];
    V4 n = vu1_ld(VU1_NRM + slot);
    float x = vu_itof(n.i[X], 12), y = vu_itof(n.i[Y], 12), z = vu_itof(n.i[Z], 12);
    int i;

    *r = n;
    for (i = 0; i < 3; i++) {
        float acc = l[0].f[i] * x;     /* mulax */
        acc = acc + l[1].f[i] * y;     /* madday */
        r->f[i] = acc + l[2].f[i] * z; /* maddz */
    }
}

/* Each group of 4 is loaded before it is stored, as the loops do */
static void normals_group(const uint32_t slot[4]) {
    V4 r[4];
    int k;

    for (k = 0; k < 4; k++) {
        normal_result(&r[k], slot[k]);
    }
    for (k = 0; k < 4; k++) {
        vu1_st(VU1_NRM + slot[k], &r[k], MXYZ);
    }
}

/* VU0 lit (sub_13B8): the listed vertices, in groups of 4. The count is
   (count - first) rounded up to 4; the padding entries transform the dummy
   vertex 127. */
void vu1_transform_normals_listed(void) {
    uint32_t first = vu1_first(), count = vu1_count();
    uint32_t end = (first + (((count - first) + 7) & 0xFC)) & 0xFFFF;
    uint32_t j, slot[4];
    int k;

    if (((end - first) & 0xFFFF) < 8) {
        vu1_warn("normals: empty vertex list", count);
        return;
    }
    for (j = first; j != ((end - 4) & 0xFFFF); j = (j + 4) & 0xFFFF) {
        for (k = 0; k < 4; k++) {
            slot[k] = list_entry(j + k);
        }
        normals_group(slot);
    }
}

/* VU1 lit (sub_1618): all vertices, count rounded up to 4 */
void vu1_transform_normals_all(void) {
    uint32_t n = ((vu1_count() + 7) & 0xFC) - 4;
    uint32_t v, slot[4];
    int k;

    if ((int32_t)n <= 0) {
        vu1_warn("normals: no vertices", vu1_count());
        return;
    }
    for (v = 0; v < n; v += 4) {
        for (k = 0; k < 4; k++) {
            slot[k] = v + k;
        }
        normals_group(slot);
    }
}

/* ---------------------------------------------------------------------- */
/* Lighting. The light block (0x180-0x18F), structure of arrays:
     0x180-0x182  the 4 directional lights' x, y, z
     0x183        their intensities
     0x184-0x186  the 4 point lights' positions, x, y, z
     0x187        the point lights' 1/range
     0x188-0x18B  the directional lights' colours
     0x18C-0x18F  the point lights' colours times intensity
   Per vertex: colour + sum C_dir max(0, I (N.L)) + sum C_pt max(0, (N.d)(1/|d| - 1))
   with d the vector to the light over its range, i.e. (N.dhat)(1 - dist/range).
   The ambient colour is added later, by the projection. */

typedef struct Lights {
    V4 dir[3];    /* 0x180-0x182 */
    V4 intens;    /* 0x183 */
    V4 pos[3];    /* 0x184-0x186 */
    V4 rrange;    /* 0x187 */
    V4 cdir[4];   /* 0x188-0x18B */
    V4 cpt[4];    /* 0x18C-0x18F */
} Lights;

static void load_lights(Lights *l) {
    int i;
    for (i = 0; i < 3; i++) {
        l->dir[i] = vu1_ld(VU1_LIGHT + i);
        l->pos[i] = vu1_ld(VU1_LIGHT + 4 + i);
    }
    l->intens = vu1_ld(VU1_LIGHT + 3);
    l->rrange = vu1_ld(VU1_LIGHT + 7);
    for (i = 0; i < 4; i++) {
        l->cdir[i] = vu1_ld(VU1_LIGHT + 8 + i);
        l->cpt[i] = vu1_ld(VU1_LIGHT + 12 + i);
    }
}

/* The 4 directional terms, max(0, (I nx) Lx + (I ny) Ly + (I nz) Lz) */
static void dir_terms(float a[4], const Lights *l, const V4 *n) {
    int i;
    for (i = 0; i < 4; i++) {
        float tx = l->intens.f[i] * n->f[X]; /* mulx vf01, vf24, vf07x */
        float ty = l->intens.f[i] * n->f[Y];
        float tz = l->intens.f[i] * n->f[Z];
        float acc = tx * l->dir[0].f[i];  /* mula */
        acc = acc + ty * l->dir[1].f[i]; /* madda */
        a[i] = maxf(acc + tz * l->dir[2].f[i], 0.0f); /* madd, max */
    }
}

/* Colour + the directional terms (+ the point terms): the ACC chain */
static void light_store(uint32_t v, const Lights *l, const float a[4], const float *pt) {
    V4 c = vu1_ld(VU1_COL + v), r;
    int k;

    for (k = 0; k < 4; k++) {
        float acc = vu_itof(c.i[k], 0) + 0.0f; /* itof0, adda with vf31 */
        acc = acc + l->cdir[0].f[k] * a[0];
        acc = acc + l->cdir[1].f[k] * a[1];
        acc = acc + l->cdir[2].f[k] * a[2];
        if (pt == NULL) {
            r.f[k] = acc + l->cdir[3].f[k] * a[3];
        } else {
            acc = acc + l->cdir[3].f[k] * a[3];
            acc = acc + l->cpt[0].f[k] * pt[0];
            acc = acc + l->cpt[1].f[k] * pt[1];
            acc = acc + l->cpt[2].f[k] * pt[2];
            r.f[k] = acc + l->cpt[3].f[k] * pt[3];
        }
    }
    vu1_st(VU1_COL + v, &r, MXYZW);
}

/* Directional lights only (L_1D20, L_2510) */
static void light_dir(uint32_t v, const Lights *l) {
    V4 n = vu1_ld(VU1_NRM + v);
    float a[4];

    dir_terms(a, l, &n);
    light_store(v, l, a, NULL);
}

/* Directional and point lights (sub_1790, sub_1F88). The loop computes
   1/|d| by RSQRT through Q for lights 0-2 and ERSQRT through P for light
   3, reading each 13 (Q) or 24 (P) pairs after issue: the new values. */
static void light_point(uint32_t v, const Lights *l) {
    V4 p = vu1_ld(VU1_POS + v), n = vu1_ld(VU1_NRM + v);
    float a[4], pt[4];
    int i;

    dir_terms(a, l, &n);
    for (i = 0; i < 4; i++) {
        float dx = (l->pos[0].f[i] - p.f[X]) * l->rrange.f[i]; /* subx, mul by 1/range */
        float dy = (l->pos[1].f[i] - p.f[Y]) * l->rrange.f[i];
        float dz = (l->pos[2].f[i] - p.f[Z]) * l->rrange.f[i];
        float dd = dx * dx;
        float nd, q, acc;
        dd = dd + dy * dy;
        dd = dd + dz * dz;
        nd = n.f[X] * dx; /* vf13 = 0 + nx, mula */
        nd = nd + n.f[Y] * dy;
        nd = maxf(nd + n.f[Z] * dz, 0.0f);
        q = vu_rsqrt(1.0f, dd);
        acc = 0.0f - nd;                    /* suba vf31, nd */
        pt[i] = maxf(vu_fix(acc + nd * q), 0.0f); /* madd by 1/|d|, max */
    }
    light_store(v, l, a, pt);
}

/* The specular pass (L_26D0, L_28B8), when the header's factor (vf30.w)
   isn't zero. With L = directional light 0 and P the view-space position:
       r = max(0, P.(-L) - 2 (P.N)(N.(-L)))     the reflection of -L, dotted with P
       alpha = factor (-I0) (r^2 / |P|^2)^2
   written over the vertex alpha. It also zeroes the ambient alpha (vf20.w)
   and leaves factor x (-I0) in vf30.w. */
static void specular_setup(float nl[3]) {
    V4 *vf = grx_vu1.vf;
    nl[0] = 0.0f - vu1_ld(VU1_LIGHT + 0).f[X];
    nl[1] = 0.0f - vu1_ld(VU1_LIGHT + 1).f[X];
    nl[2] = 0.0f - vu1_ld(VU1_LIGHT + 2).f[X];
    vf[VF_AMBIENT].f[W] = 1.0f * 0.0f; /* mulx.w vf20, vf00, vf00x */
    vf[VF_UVOFF].f[W] = vf[VF_UVOFF].f[W] * (0.0f - vu1_ld(VU1_LIGHT + 3).f[X]);
}

static void specular_vertex(uint32_t v, const float nl[3]) {
    V4 p = vu1_ld(VU1_POS + v), n = vu1_ld(VU1_NRM + v), r;
    float pn, nL, pp, pl, acc, s, t;

    pn = p.f[X] * n.f[X] + p.f[Y] * n.f[Y]; /* adday.x */
    pn = pn + 1.0f * (p.f[Z] * n.f[Z]);     /* maddz.x by vf01.x = 1 */
    nL = n.f[X] * nl[0] + n.f[Y] * nl[1];
    nL = nL + 1.0f * (n.f[Z] * nl[2]);
    pp = p.f[X] * p.f[X] + p.f[Y] * p.f[Y];
    pp = pp + 1.0f * (p.f[Z] * p.f[Z]);
    pl = p.f[X] * nl[0] + p.f[Y] * nl[1];
    pl = pl + 1.0f * (p.f[Z] * nl[2]);
    acc = pl - pn * nL; /* msuba.x */
    s = acc - pn * nL;  /* msub.x */
    s = maxf(s, 0.0f);
    t = s * s;
    t = vu_fix(t * vu_div(1.0f, pp)); /* mulq by 1/|P|^2 */
    t = t * t;
    r.f[W] = vu_fix(grx_vu1.vf[VF_UVOFF].f[W] * t);
    vu1_st(VU1_COL + v, &r, MW);
}

/* Whether the point lights are off: all four 1/range = 1, the reset value
   (subw vf01, vf28, vf00w; fmand 0xF) */
static int point_lights_off(const Lights *l) {
    int i;
    for (i = 0; i < 4; i++) {
        if (l->rrange.f[i] - 1.0f != 0.0f) {
            return 0;
        }
    }
    return 1;
}

static int specular_on(void) {
    return grx_vu1.vf[VF_UVOFF].f[W] + 0.0f != 0.0f; /* addw.x vf01, vf00, vf30w; fmand 8 */
}

/* VU0 lit (sub_1790): the listed vertices [first, count) */
void vu1_light_listed(void) {
    uint32_t first = vu1_first(), count = vu1_count(), j;
    Lights l;
    float nl[3];

    load_lights(&l);
    if (point_lights_off(&l)) {
        for (j = first; j != count; j = (j + 1) & 0xFFFF) {
            light_dir(list_entry(j), &l);
        }
    } else {
        for (j = first; j != count; j = (j + 1) & 0xFFFF) {
            light_point(list_entry(j), &l);
        }
    }
    if (specular_on()) {
        specular_setup(nl);
        for (j = first; j != count; j = (j + 1) & 0xFFFF) {
            specular_vertex(list_entry(j), nl);
        }
    }
}

/* VU1 lit (sub_1F88): all vertices [0, count). The directional-only loop
   (L_2510) runs one vertex further, to count inclusive: it ends on
   vi01 == count + 3 where the others end on count + 2. The extra slot is
   past the mesh's vertices, so nothing reads it. */
void vu1_light_all(void) {
    uint32_t count = vu1_count(), v;
    Lights l;
    float nl[3];

    load_lights(&l);
    if (point_lights_off(&l)) {
        for (v = 0; v <= count; v++) {
            light_dir(v, &l);
        }
    } else {
        for (v = 0; v < count; v++) {
            light_point(v, &l);
        }
    }
    if (specular_on()) {
        specular_setup(nl);
        for (v = 0; v < count; v++) {
            specular_vertex(v, nl);
        }
    }
}

/* ---------------------------------------------------------------------- */
/* Projection (L_0578 for the listed vertices, L_0810 for all). Per vertex,
   with q = distance / z (vf18.w / z):
     XYZ  = offset.xyz + scale.xyz (x, y, 1) q              12.4 X, Y; 1/z depth
     F    = 16 (fog bias + fog2) + fog1 fog0^2 16 / (fog0^2 + |p|^2)
     STQ  = ((uv.xy from 4.12) x 4 + uvoff.xy, uv.z) q      uv.z = 1.0 from STROW
     RGBA = clamp(colour + ambient, 0, 255)
   XYZF and RGBA go through ftoi0 and are written over the position and the
   colour, STQ over the UV. The loop issues the fog DIV in the pair where
   maddq still reads the previous DIV's q = d/z, and reads the fog quotient
   7 pairs later: both are the values meant here. */

static void project_setup(void) {
    V4 *vf = grx_vu1.vf;
    vf[VF_OFFSET].f[W] = (vf[VF_OFFSET].f[W] + vf[VF_FOG].f[X]) * 16.0f; /* addx.w, muli.w by I = 16 */
}

static void project_vertex(uint32_t v) {
    const V4 *vf = grx_vu1.vf;
    const V4 *s = &vf[VF_PROJ], *o = &vf[VF_OFFSET], *fog = &vf[VF_FOG];
    V4 p = vu1_ld(VU1_POS + v), uv = vu1_ld(VU1_NRM + v), c = vu1_ld(VU1_COL + v);
    V4 stq, xyzf, rgba;
    float q, qf, den, u, t, sx, sy, sz, cx;
    int k;

    /* STQ */
    u = vu_itof(uv.i[X], 12) * 4.0f + vf[VF_UVOFF].f[X];
    t = vu_itof(uv.i[Y], 12) * 4.0f + vf[VF_UVOFF].f[Y];
    q = vu_div(vf[VF_COUNTS].f[W], p.f[Z]);
    stq.f[X] = vu_fix(u * q);
    stq.f[Y] = vu_fix(t * q);
    stq.f[Z] = vu_fix(uv.f[Z] * q);
    /* fog: fog0^2 + x^2 + y^2 + z^2 (addax.w, madday.w, maddz.w) */
    sx = p.f[X] * p.f[X];
    sy = p.f[Y] * p.f[Y];
    sz = p.f[Z] * p.f[Z];
    den = fog->f[W] + sx;
    den = den + 1.0f * sy;
    den = den + 1.0f * sz;
    qf = vu_div(fog->f[Y], den);
    /* XYZF: ACC = offset (adda vf17 + vf31), then maddq */
    xyzf.i[X] = vu_ftoi(vu_fix(o->f[X] + vu_fix(s->f[X] * p.f[X]) * q), 0);
    xyzf.i[Y] = vu_ftoi(vu_fix(o->f[Y] + vu_fix(s->f[Y] * p.f[Y]) * q), 0);
    xyzf.i[Z] = vu_ftoi(vu_fix(o->f[Z] + (s->f[Z] * 1.0f) * q), 0);
    xyzf.i[W] = vu_ftoi(o->f[W] + qf, 0);
    /* RGBA (add, maxz with vf00z = 0, miniz with vf29z = 255, ftoi0) */
    for (k = 0; k < 4; k++) {
        cx = c.f[k] + vf[VF_AMBIENT].f[k];
        cx = minf(maxf(cx, 0.0f), fog->f[Z]);
        rgba.i[k] = vu_ftoi(cx, 0);
    }
    vu1_st(VU1_NRM + v, &stq, MXYZ);
    vu1_st(VU1_COL + v, &rgba, MXYZW);
    vu1_st(VU1_POS + v, &xyzf, MXYZW);
}

/* The VU0 paths: the listed vertices [first, count). The XYZF written over
   a position replaces its list entry (w); GphVertCompact lists vertices so
   that entry j names a vertex <= j, which is never ahead of the loop. */
void vu1_project_listed(void) {
    uint32_t first = vu1_first(), count = vu1_count(), j;

    project_setup();
    if (first == count) {
        vu1_warn("project: empty vertex list", count);
        return;
    }
    for (j = first; j != count; j = (j + 1) & 0xFFFF) {
        project_vertex(list_entry(j));
    }
}

/* The VU1 paths: every vertex [0, count) */
void vu1_project_all(void) {
    uint32_t count = vu1_count(), v;

    project_setup();
    for (v = 0; v < count; v++) {
        project_vertex(v);
    }
}
