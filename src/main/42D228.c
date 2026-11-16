#include "common.h"
#include "main/n03.h"

void McCrcTableInit(void) {
    s32 i;
    s32 j;
    u32 c;
    for (i = 0; i < 0x100; i++) {
        c = i;
        for (j = 0; j < 8; j++) {
            if (c & 1) {
                c = (c >> 1) ^ 0xEDB88320;
            } else {
                c >>= 1;
            }
        }
        mcCrcTable[i] = c;
    }
}
