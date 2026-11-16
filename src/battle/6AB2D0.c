#include "common.h"

f32 BtlMotionKeyGetFrame(s16 *p, s32 arg1) {
    while (p != NULL && *p >= 0) {
        f32 v = *p++;
        if (*p++ == arg1) {
            return v;
        }
        p += *p + 1;
    }
    return 0.0f;
}
