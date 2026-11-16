#include "common.h"
#include "protos/battle.h"

extern char btlDbgStrFmt[];
extern char btlDbgBitOnStr[];
extern char btlDbgBitOffStr[];

void BtlDbgPrintBits(s32 x, s32 y, s32 bits, s32 n) {
    s32 m;

    for (m = 1 << (n - 1); m > 0; m >>= 1) {
        DbgPrintf(x, y, btlDbgStrFmt, (m & bits) ? btlDbgBitOnStr : btlDbgBitOffStr);
        x++;
    }
}
