#ifndef VU_H
#define VU_H

#include "common.h"

/* VU0 vector types (libvu0's sceVu0FVECTOR / sceVu0IVECTOR / sceVu0FMATRIX),
 * shared by every header that needs them. */
#ifndef VU_TYPES
#define VU_TYPES
#define FVECTOR_DEFINED
typedef f32 FVECTOR[4] __attribute__((aligned(16)));
typedef s32 IVECTOR[4] __attribute__((aligned(16)));
typedef f32 FMATRIX[4][4] __attribute__((aligned(16)));
#endif

/* Plain float vectors as structs (not 16-byte aligned, unlike FVECTOR): the
 * prim, sound, model and particle code pass them around and embed them in
 * records at 4-byte offsets. */
#ifndef VU_VEC3_DEFINED
#define VU_VEC3_DEFINED
typedef struct VuVec3 {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
} VuVec3; /* size 0xC */
#endif

#ifndef VU_VEC4_DEFINED
#define VU_VEC4_DEFINED
typedef struct VuVec4 {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
    /* 0xC */ f32 w;
} VuVec4; /* size 0x10 */
#endif

#endif
