#include "common.h"
#include "field/n25.h"
#include "protos/field.h"

void MiniSparkleSpawn(f32 x, f32 y) {
    MiniSparkle *p;
    f32 r;

    miniSparkleIndex++;
    if (miniSparkleIndex >= 0x20) {
        miniSparkleIndex = 0;
    }
    p = &miniSparkles[miniSparkleIndex];
    p->x = x + VuRandSigned(1.0f) * 24.0f;
    r = VuRandSigned(1.0f);
    p->speed = -0.4f;
    p->y = y + r * 24.0f;
    p->angle = rand() >> 15;
    p->timer = 0xA0;
}
