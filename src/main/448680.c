#include "common.h"

void FilePof0Apply(s32 *base, u8 *p, s32 size) {
    u8 *end;
    u8 b;
    u32 c;
    s32 off;
    s32 *ptr;
    s32 t;

    b = *p;
    c = b;
    off = 0;
    if (c == 0) {
        return;
    }
    end = p + size;
    /* goto loop: a while/for loop adds the loop pass's rotated test (3 more insns) */
loop:
    if (p < end) {
        switch (c & 0xC0) {
        case 0x40:
            off += c & 0x3F;
            ptr = &base[off];
            if (*ptr != 0) {
                *ptr += (s32)base;
            }
            p += 1;
            break;
        case 0x80:
            t = ((c & 0x3F) << 8) + p[1];
            off += t;
            ptr = &base[off];
            if (*ptr != 0) {
                *ptr += (s32)base;
            }
            p += 2;
            break;
        case 0xC0:
            t = (((c & 0x3F) << 24) + p[3]) + ((p[2] << 8) + (p[1] << 16));
            off += t;
            ptr = &base[off];
            if (*ptr != 0) {
                *ptr += (s32)base;
            }
            p += 4;
            break;
        }
        c = *p;
        if (c != 0) {
            goto loop;
        }
    }
}
