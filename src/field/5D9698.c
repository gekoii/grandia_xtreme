#include "common.h"

void DbgFldBitSet(u8 *arg0, u16 arg1, s32 arg2) {
    u8 *p = arg0 + (arg1 >> 3);
    *p = (*p & ~(1 << (arg1 & 7))) | ((arg2 & 1) << (arg1 & 7));
}
