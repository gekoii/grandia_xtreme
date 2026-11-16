#include "common.h"

f32 CamScrArgFloat(s32 arg0) {
    union { s32 i; f32 f; } u;
    u.i = arg0;
    return u.f;
}
