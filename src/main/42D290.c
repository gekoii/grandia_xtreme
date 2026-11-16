#include "common.h"
#include "main/n03.h"

u32 McCrcUpdate(u32 crc, u8 *buf, u32 len) {
    u32 i;
    u32 c = crc;
    for (i = 0; i < len; i++) {
        c = mcCrcTable[(c ^ buf[i]) & 0xFF] ^ (c >> 8);
    }
    return c;
}
