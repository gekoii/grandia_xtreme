#include "common.h"

void ChrMotFlagsSet(u32 *flags, u8 bits, u8 set) {
    if (set) {
        if (bits & 1) {
            *flags |= 1;
        }
        if (bits & 2) {
            *flags |= 2;
        }
        if (bits & 4) {
            *flags |= 4;
        }
        if (bits & 8) {
            *flags |= 8;
        }
    } else {
        if ((bits & 1) && (*flags & 1)) {
            *flags &= ~1;
        }
        if ((bits & 2) && (*flags & 2)) {
            *flags &= ~2;
        }
        if ((bits & 4) && (*flags & 4)) {
            *flags &= ~4;
        }
        if ((bits & 8) && (*flags & 8)) {
            *flags &= ~8;
        }
    }
}
