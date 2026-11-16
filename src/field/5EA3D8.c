#include "common.h"
#include "protos/field.h"


s32 TxtGlyphQueueStr(u8 id, u8 *s, s32 pos) {
    s32 i;
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    u16 c;
    u16 r;

    if (pos >= 14) {
        y = pos - 14;
        x = 14;
    } else {
        y = pos;
        x = 0;
    }
    i = 0;
    while (*s != 0) {
        if (i < 28) {
            dx = i;
            dy = 0;
        } else {
            dx = i - 28;
            dy = 1;
        }
        if (s[1] >= 0xF0) {
            c = s[1];
            c <<= 8;
            c |= s[0];
            s += 2;
            r = TxtMsgCharToGlyph(c);
        } else {
            r = TxtMsgCharToGlyph(*s++);
        }
        i++;
        TxtGlyphQueuePush(id, r, x + dx, y + dy);
    }
    return i;
}
