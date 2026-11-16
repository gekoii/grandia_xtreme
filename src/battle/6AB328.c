#include "common.h"

s32 BtlMotionKeyCount(s16 *p, s32 arg1) {
    s32 n = 0;
    while (p != NULL && *p >= 0) {
        p++;
        if (*p++ == arg1) {
            n++;
        }
        p += *p + 1;
    }
    return n;
}
