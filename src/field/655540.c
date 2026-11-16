#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

void WmapDrawWarp(void) {
    u8 n;

    n = wmapWork->active;
    if (n != 0) {
        if (wmapEntering != 0) {
            ((s32 (*)(u8))WmapWarpDraw)(n);
        }
    }
}
