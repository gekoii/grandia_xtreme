/* The VU1 microprogram in C (port/src/vu1/): shared state and helpers. Not
   for the game; the interface is port/include/grx_vu1.h, the program is
   described in port/docs/vu.md.

   The game has one VU1 program (vuMicroCode1): a mesh renderer run in
   phases, each ended by an E bit and continued by the VIF with MSCNT. Each
   stage of it is a C function here, working on VU1 data memory
   (grx_vu1_data) and on the VF registers the program keeps between phases
   (the header it loads into vf16-vf30). Addresses in comments are micro
   memory byte addresses of the original (vu_disasm.py's listing).

   Arithmetic follows the microcode operation by operation (same operands,
   same order, single precision, no fused multiply-add), so results differ
   from the hardware only by its rounding (the VU truncates). Where the
   pipelined loops read Q, P or the flags in a slot where the hardware still
   holds an older value, the code here uses the value the program means
   (port/docs/vu.md); those places are noted. */
#ifndef VU1_INTERNAL_H
#define VU1_INTERNAL_H

#include <stdint.h>
#include <string.h>
#include "grx_vu1.h"

#pragma STDC FP_CONTRACT OFF

typedef union V4 {
    float f[4];
    int32_t i[4];
    uint32_t u[4];
} V4;

enum { X = 0, Y = 1, Z = 2, W = 3 };

/* Write masks: bit 0 = x ... bit 3 = w */
enum { MX = 1, MY = 2, MZ = 4, MW = 8, MXY = 3, MXYZ = 7, MXYZW = 15 };

/* VU1 data memory in quadwords; addresses wrap at 16 KB as on the VU */
#define VU1_QW_MASK 0x3FFu

static inline V4 vu1_ld(uint32_t qa) {
    V4 v;
    memcpy(&v, grx_vu1_data + ((qa & VU1_QW_MASK) << 4), 16);
    return v;
}

static inline void vu1_st(uint32_t qa, const V4 *v, int mask) {
    uint8_t *p = grx_vu1_data + ((qa & VU1_QW_MASK) << 4);
    int i;
    if (mask == MXYZW) {
        memcpy(p, v, 16);
        return;
    }
    for (i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            memcpy(p + 4 * i, &v->u[i], 4);
        }
    }
}

/* ilw: the low 16 bits of a word; isw: a 16-bit value, zero-extended */
static inline uint32_t vu1_ilw(uint32_t qa, int field) {
    uint32_t w;
    memcpy(&w, grx_vu1_data + ((qa & VU1_QW_MASK) << 4) + 4 * field, 4);
    return w & 0xFFFFu;
}

static inline void vu1_isw(uint32_t qa, int field, uint32_t value) {
    uint32_t w = value & 0xFFFFu;
    memcpy(grx_vu1_data + ((qa & VU1_QW_MASK) << 4) + 4 * field, &w, 4);
}

/* mtir: the low 16 bits of a register field */
static inline uint32_t vu1_mtir(const V4 *v, int field) {
    return v->u[field] & 0xFFFFu;
}

/* The VU has no infinities, NaNs or denormals: results saturate to
   +-FLT_MAX and tiny ones flush to zero */
static inline float vu_fix(float x) {
    uint32_t b;
    memcpy(&b, &x, 4);
    if ((b & 0x7F800000u) == 0x7F800000u) {
        b = (b & 0x80000000u) | 0x7F7FFFFFu;
    } else if ((b & 0x7F800000u) == 0) {
        b &= 0x80000000u;
    }
    memcpy(&x, &b, 4);
    return x;
}

/* DIV and RSQRT; division by zero gives +-FLT_MAX as on the VU */
static inline float vu_div(float a, float b) {
    if (b == 0.0f) {
        uint32_t s, r;
        float x;
        memcpy(&s, &a, 4);
        memcpy(&r, &b, 4);
        r = ((s ^ r) & 0x80000000u) | 0x7F7FFFFFu;
        if (a == 0.0f) {
            r = 0x7F7FFFFFu; /* 0/0: the VU gives +MAX */
        }
        memcpy(&x, &r, 4);
        return x;
    }
    return vu_fix(a / b);
}

static inline float vu_rsqrt(float a, float b) {
    float s = b < 0.0f ? -b : b; /* the VU takes |ft| */
    if (s == 0.0f) {
        return a < 0.0f ? -3.40282347e38f : 3.40282347e38f;
    }
    return vu_fix(a / __builtin_sqrtf(s));
}

/* itofN: integer to float with N fraction bits; ftoiN back, truncating
   toward zero and saturating */
static inline float vu_itof(int32_t i, int frac) {
    return (float)i * (1.0f / (float)(1 << frac));
}

static inline int32_t vu_ftoi(float x, int frac) {
    double d;
    if (x != x) {
        return 0;
    }
    d = (double)x * (double)(1 << frac);
    if (d >= 2147483647.0) {
        return 0x7FFFFFFF;
    }
    if (d <= -2147483648.0) {
        return (int32_t)0x80000000u;
    }
    return (int32_t)d;
}

/* The VF registers the program keeps between phases, by their hardware
   numbers: vf16 projection scale, vf17 screen offset (w: fog bias), vf18
   (first used slot, vertex count, triangle count, projection distance),
   vf19 the GIF tag, vf20 the ambient colour, vf21-vf23 the light
   (normal) matrix, vf24-vf27 the model-view matrix, vf29 the fog
   constants (fog2, fog1 fog0^2 16, 255, fog0^2), vf30 the UV offset (w:
   the specular factor), vf31 zero. vf06.w and vf07.w carry the sort
   keys' sum and count from the key pass to the sort. */
enum {
    VF_KEYSUM = 6,
    VF_KEYCNT = 7,
    VF_PROJ = 16,
    VF_OFFSET = 17,
    VF_COUNTS = 18,
    VF_GIFTAG = 19,
    VF_AMBIENT = 20,
    VF_LMTX = 21,
    VF_MV = 24,
    VF_FOG = 29,
    VF_UVOFF = 30,
    VF_ZERO = 31
};

typedef struct Vu1State {
    V4 vf[32];
    uint32_t tri_stage; /* vi15: the triangle stage the entry chose (byte address) */
    uint32_t sort_ret;  /* vi14 in the key pass: the end the sort returns past */
} Vu1State;

extern Vu1State grx_vu1;

/* The counts in vf18 (mtir reads 16 bits) */
static inline uint32_t vu1_first(void) { return vu1_mtir(&grx_vu1.vf[VF_COUNTS], X); }
static inline uint32_t vu1_count(void) { return vu1_mtir(&grx_vu1.vf[VF_COUNTS], Y); }
static inline uint32_t vu1_ntri(void) { return vu1_mtir(&grx_vu1.vf[VF_COUNTS], Z); }

/* VU memory map (quadwords) */
#define VU1_POS 0x000u     /* positions, then XYZF */
#define VU1_NRM 0x080u     /* normals, then UVs, then STQ */
#define VU1_COL 0x100u     /* colours, then RGBA */
#define VU1_TRI 0x17Fu     /* the triangle list */
#define VU1_LIGHT 0x180u   /* the light block */
#define VU1_SAVE 0x190u    /* lighting's save area for vf16-vf20, vf29, vf30 */
#define VU1_HEADER 0x1A0u  /* the per-mesh header */
#define VU1_GIFTAG 0x1BFu  /* the GIF tag the packet is kicked from */
#define VU1_PACKET 0x1C0u  /* the GIF packet */
#define VU1_PAD_INDEX 0x7Fu /* the dummy vertex the loops pad lists with */

/* Header layouts (entry points): which registers each loads */
enum { HDR_VU0_UNLIT, HDR_VU0_LIT, HDR_VU1_UNLIT, HDR_VU1_LIT };

/* vu1_vertex.c: the vertex stages */
void vu1_load_header(int layout);
void vu1_pad_list(void);
void vu1_colours_to_float(uint32_t first_qw, uint32_t n);
void vu1_transform_positions(uint32_t n);
void vu1_transform_normals_listed(void);
void vu1_transform_normals_all(void);
void vu1_light_listed(void);
void vu1_light_all(void);
void vu1_project_listed(void);
void vu1_project_all(void);

/* vu1_tri.c: the triangle stages */
void vu1_sort_keys_vu0(void);
void vu1_sort_keys_vu1(void);
void vu1_sort(void);
void vu1_assemble(int vu1_indices, int cull);
void vu1_kick(uint32_t qa);

/* Diagnostics, each once */
void vu1_warn(const char *what, uint32_t value);

#endif
