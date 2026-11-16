#include "common.h"
#include "m2c_macros.h"

void GphRippleModuleInit(void) {

}

s32 GphRippleWorkSize(u16 arg0, u16 arg1) {
    return (arg0 * (arg1 + 4) * 8) + 0x24;
}
