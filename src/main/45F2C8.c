#include "common.h"
#include "protos/main.h"

extern u8 sysIopModuleArgs[];

void SysLoadIopModule(s32 arg0) {
    s32 i;

    while (sceSifLoadModule(arg0, 0, sysIopModuleArgs) < 0) {
        i = 0x400000;
        while (--i != 0) {
        }
    }
    i = 0x100000;
    while (--i != 0) {
    }
}
