/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void _InvalidDCache(u32, u32);

void iInvalidDCache(u32 a0, u32 a1) {
    _InvalidDCache(a0 & 0xFFFFFFC0, a1 & 0xFFFFFFC0);
}
