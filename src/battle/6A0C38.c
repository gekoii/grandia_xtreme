/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n30.h"

void FxCastSparkUpdate(void) {
    FxCastSpark *p = fxCastSparks;
    s32 i;

    for (i = 0; i < 50; i++, p++) {
        if (p->life > 0) {
            p->pos.x += p->vel.x;
            p->pos.y += p->vel.y;
            p->pos.z += p->vel.z;
            p->vel.x += p->accel.x;
            p->vel.y += p->accel.y;
            p->vel.z += p->accel.z;
            if (p->age < 10) {
                p->alpha += 0.3f;
            } else if (p->age < 20) {
                p->alpha -= 0.2f;
            } else {
                p->alpha = (f32)(p->life - p->age) / (f32)(p->life - 20);
            }
            p->age++;
            if (p->age >= p->life) {
                p->life = 0;
                fxCastSparkCount--;
            }
        }
    }
}
