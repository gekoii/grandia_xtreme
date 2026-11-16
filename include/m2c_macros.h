#ifndef M2C_MACROS_H
#define M2C_MACROS_H

/* Minimal definitions for the macros m2c emits with --valid-syntax.
 * Anything m2c could not translate (M2C_ERROR etc.) is left undefined on
 * purpose, so that code fails to compile until it is fixed by hand. */

#include "common.h"

typedef s32 M2C_UNK;
typedef s8 M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

#define M2C_FIELD(expr, type, offset) (*(type)((u8 *)(expr) + (offset)))
#define M2C_BITWISE(type, expr) ((type)(expr))
#define M2C_STRUCT_COPY(dst, src) (*(dst) = *(src))

#endif
