/* cc: ee-gcc2.9-991111-01 */
#include "common.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec;

void AmapPosToCell(Vec *arg0, s32 *arg1, s32 *arg2) {
    *arg1 = (s32)((arg0->x - 250.0f) / 500.0f) + 1;
    *arg2 = (s32)-((arg0->z + 250.0f) / 500.0f - 1.0f);
}
