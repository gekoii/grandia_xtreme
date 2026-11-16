#include "common.h"
#include "field/n15.h"

extern u16 mapVariantTable[];

s32 MapHasVariant(u16 arg0) {
    s32 i;
    s32 hi;

    if (mapVariantNo == 1) {
        hi = arg0 & 0xFF00;
        if (hi == 0x7000 || hi == 0x8000 || hi == 0x9000) {
            return 0;
        }
    }
    for (i = 0; i < 0x24; i++) {
        if (arg0 == mapVariantTable[i]) {
            return 1;
        }
    }
    return 0;
}
