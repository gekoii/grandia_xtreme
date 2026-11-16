#include "common.h"
#include "protos/main.h"


u32 McCrc32(u8 *buf, u32 len) {
    return ~McCrcUpdate(0xFFFFFFFF, buf, len);
}
