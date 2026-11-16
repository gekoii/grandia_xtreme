#include "common.h"
#include "grxsys.h"
#include "protos/field.h"




s32 TxtStrGetWidth(u8 *s, s32 size) {
    s8 m[2];
    s32 w;
    s32 x;
    s32 c;

    w = 0;

    while (*s != 0) {
        if (s[1] >= 0xF0) {
            c = s[0] | (s[1] << 8);
            s += 2;
            TxtGlyphGetMetrics((u16)TxtMsgCharToGlyph(c), (u8 *)&m[0], (u8 *)&m[1], sysGrxsysPac->glyphTable);
        } else {
            c = s[0];
            s++;
            TxtGlyphGetMetrics((u16)TxtMsgCharToGlyph(c), (u8 *)&m[0], (u8 *)&m[1], sysGrxsysPac->glyphTable);
        }
        x = w + 2;
        w = x + (s32)((f32)(m[0] - m[1]) * (f32)size * 0.0625f);
    }
    return w;
}
