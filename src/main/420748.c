#include "common.h"
#include "main/n02.h"

typedef struct { f32 x, y, z; } Vec3;

void DbgLightAngleGet(u8 arg0, Vec3 *arg1) {
    DbgLightAngle *v;
    DbgLightAngle *base = dbgLightAngles;
    v = base + arg0;
    arg1->x = v->x; arg1->y = v->y; arg1->z = v->z;
}
