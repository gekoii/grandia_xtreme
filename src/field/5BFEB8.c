#include "common.h"
#include "protos/field.h"


f32 OccEase(s32 mode, f32 t) {
    switch (mode) {
    case 0:
        break;
    case 1:
        t = (-cosf(t * 3.141592f) + 1.0f) * 0.5f;
        break;
    case 2:
        t = -cosf(t * 1.570796f) + 1.0f;
        break;
    case 3:
        t = sinf(t * 1.570796f);
        break;
    }
    return t;
}
