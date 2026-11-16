#include "common.h"
#include "field/n14.h"

s32 ChrTenantAreaSetCount(u32 arg0) {
    if (arg0 >= 0x9EU) {
        chrTenantAreaTable->count = 0x9D;
        return 0;
    }
    chrTenantAreaTable->count = arg0;
    return 1;
}
