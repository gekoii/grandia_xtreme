#include "common.h"

u8 *EvtScnrReadU32(u8 *p, s32 *out) {
    *out = *p++;
    *out |= *p++ << 8;
    *out |= *p++ << 16;
    *out |= *p++ << 24;
    return p;
}
