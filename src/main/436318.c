#include "common.h"
s32 GphDrawListSize(s32 arg0) {
    s32 t = (arg0 << 3) + (arg0 << 1);
    return (arg0 << 4) + t + 0xD6;
}
