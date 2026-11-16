#include "common.h"
#include "field/n18.h"
#include "protos/field.h"

void TxtMsgGetCodes(u8 *arg0, s32 arg1, s32 *arg2) {
    s32 i;
    u8 *len;
    u16 *str;
    u16 c;
    u8 *s;
    s32 *out;

    if (arg1 >= 0) {
        TxtMsgHdr *h = (TxtMsgHdr *)arg0;
        len = (u8 *)(h->lenOfs + (s32)h) + arg1;
        str = *(u16 **)(arg1 * 4 + (h->strOfs + (s32)h));
        for (i = 0; i < *len; i++) {
            *arg2++ = *str++;
        }
    } else {
        s = arg0;
        out = arg2;
        while (*s != 0) {
            if (s[1] >= 0xF0) {
                c = s[1];
                c <<= 8;
                c |= s[0];
                s += 2;
            } else {
                c = s[0];
                s++;
            }
            *out++ = (u16)TxtMsgCharToGlyph(c);
        }
    }
}
