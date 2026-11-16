/* cflags: -fno-gcse */
/* -fno-gcse: without it gcse keeps the length byte *len in a register, dropping the loop's and the return's reloads */
#include "common.h"
#include "field/n18.h"
#include "protos/field.h"

u8 TxtGlyphQueueMsg(u8 id, TxtMsgHdr *h, s32 pos, s32 idx) {
    s32 i;
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    u8 *len;
    u8 *lens;
    u16 *str;

    str = *(u16 **)(idx * 4 + (h->strOfs + (s32)h));
    lens = (u8 *)(h->lenOfs + (s32)h);
    if (pos >= 14) {
        y = pos - 14;
        x = 14;
    } else {
        y = pos;
        x = 0;
    }
    len = lens + idx;
    for (i = 0; i < *len; i++) {
        if (i < 28) {
            dx = i;
            dy = 0;
        } else {
            dx = i - 28;
            dy = 1;
        }
        TxtGlyphQueuePush(id, str[i], x + dx, y + dy);
    }
    return *len;
}
