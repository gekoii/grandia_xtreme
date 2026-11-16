#include "common.h"
#include "grxsys.h"
#include "protos/field.h"




s32 TxtCodesGetWidth(u16 *str, s32 n, s32 size) {
    s8 m[2];
    s32 i;
    s32 w;
    s32 x;

    w = 0;
    for (i = 0; i < n; i++) {
        TxtGlyphGetMetrics(str[i], (u8 *)&m[0], (u8 *)&m[1], sysGrxsysPac->glyphTable);
        x = w + 2;
        w = x + (s32)((f32)(m[0] - m[1]) * (f32)size * 0.0625f);
    }
    return w;
}
