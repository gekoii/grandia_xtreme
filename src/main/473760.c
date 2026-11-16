/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"


typedef struct { u8 b[0x180]; } Row;

#define UNC(x) (((u32)(x) & 0x0FFFFFFF) | 0x20000000)

static inline u32 unc(void *p) { return UNC(p); }

void _sceMpegSetPicBufAddr(u32 *a0, u32 *a1, u32 *a2, u32 *a3, u32 *a4, u32 *a5, u32 *a6, u32 *a7, u32 *a8, Row *y, Row *u, Row *v) {
    s32 ym;
    s32 n;

    ym = (u32)y & 0x0FFFFFFF;
    n = Coded_Picture_Width * Coded_Picture_Height / 512;
    *a0 = UNC(y);
    *a1 = UNC(u);
    *a2 = UNC(v);
    *a3 = ym | 0x20000000;
    *a4 = UNC(u);
    *a5 = UNC(v);
    *a6 = unc(y + n);
    *a7 = unc(u + n);
    *a8 = unc(v + n);
}
