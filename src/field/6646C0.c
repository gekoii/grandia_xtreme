#include "common.h"

u8 *EvtScnrReadU16(u8 *p, s16 *out) {
    *out = *p++;
    *out += *p++ << 8;
    return p;
}
