#include "common.h"

s32 DbgFldBitGet(u8 *arg0, u16 arg1) {
    return (arg0[arg1 >> 3] >> (arg1 & 7)) & 1;
}
