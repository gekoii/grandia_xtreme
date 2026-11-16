#include "common.h"
#include "protos/main.h"


u32 *MovGsTrxDir(u32 *arg0, u32 arg1) {
    return MovGsAd(arg0, 0x53, arg1);
}
