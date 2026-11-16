#include "common.h"

s32 SndCheckFadeParams(u32 *arg0) {
    if (arg0[1] >= 0x100) {
        return -9;
    }
    return (arg0[2] < 0x100) ? 0 : -9;
}
