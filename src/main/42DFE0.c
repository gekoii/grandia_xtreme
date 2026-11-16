#include "common.h"
#include "main/n03.h"

void DbgTextSetColor(s32 arg0, u64 arg1) {
    dbgTextColors[arg0 & 0xFF] = arg1 | (0xFE00ULL << 46);
}
