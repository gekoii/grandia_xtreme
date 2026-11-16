#include "common.h"

void memcpy(u8 *, u8 *, s32);

s32 videoDecCopy2area(u8 *dst1, s32 len1, u8 *dst2, s32 len2, u8 *src1, s32 n1, u8 *src2, s32 n2) {
    s32 total;
    s32 rem;

    total = n1 + n2;
    if (len1 + len2 < total) {
        return 0;
    }
    if (n1 >= len1) {
        memcpy(dst1, src1, len1);
        memcpy(dst2, src1 + len1, n1 - len1);
        memcpy(dst2 + n1 - len1, src2, n2);
    } else {
        rem = len1 - n1;
        if (n2 >= rem) {
            memcpy(dst1, src1, n1);
            memcpy(dst1 + n1, src2, rem);
            memcpy(dst2, src2 + len1 - n1, n2 - rem);
        } else {
            memcpy(dst1, src1, n1);
            memcpy(dst1 + n1, src2, n2);
        }
    }
    return total;
}
