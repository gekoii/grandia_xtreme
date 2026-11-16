#include "common.h"
#include "protos/main.h"
#include "grxsys.h"




s32 TxtStringWidth(void *arg0, u8 *s, s8 spacing) {
    s8 m[2];
    s32 total;
    s32 n;
    u16 c;

    total = 0;
    n = 0;
    while (*s != 0) {
        if (s[1] >= 0xF0) {
            c = s[1];
            c <<= 8;
            c = s[0] | c;
            s++;
        } else {
            c = s[0];
        }
        if (arg0 != NULL) {
            TxtGlyphBufPut(arg0, c);
        }
        s++;
        n++;
        TxtGlyphGetMetrics(TxtCharToGlyph(c), (u8 *)&m[0], (u8 *)&m[1], sysGrxsysPac->glyphTable);
        total += m[0] - m[1];
    }
    return total * 1.5f + (f32)((n - 1) * spacing) + 3.0f;
}
