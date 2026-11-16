/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x00703428-0x00703438 */
#include "common.h"
#include "viewer/n35.h"
#include "protos/viewer.h"

void VwrFlockInit(s16 a, u16 count, s16 c) {
    s32 i;
    VwrFlockAgent *p;

    vwrFlockCfg.firstModel = a;
    vwrFlockCfg.count = count;
    vwrFlockCfg.on = 1;
    vwrFlockCfg.waypointCount = c;
    for (i = 0; i < vwrFlockCfg.count; i++) {
        p = &vwrFlockAgents[i];
        p->slow = 0;
        p->avoidTimer = 0;
        p->sepRadius = VuRandSigned(2147483648.0f) * 0.5f * 10.0f + 150.0f;
        p->maxSpeed = VuRandSigned(2147483648.0f) * 0.5f * 15.0f + 15.0f;
        if (p->maxSpeed < 0.0f) {
            p->maxSpeed = -p->maxSpeed;
        }
        p->minSpeed = VuRandSigned(2147483648.0f) * 0.5f * 0.6f + 0.6f;
        if (p->minSpeed < 0.0f) {
            p->minSpeed = -p->minSpeed;
        }
        p->accel = VuRandSigned(2147483648.0f) * 0.5f * 0.015f + 0.015f;
        if (p->accel < 0.0f) {
            p->accel = -p->accel;
        }
        p->climb = VuRandSigned(2147483648.0f) * 0.5f * 1.5f + 1.5f;
        if (p->climb < 0.0f) {
            p->climb = -p->climb;
        }
        p->climbAccel = VuRandSigned(2147483648.0f) * 0.5f * 0.05 + 0.05f;
        if (p->climbAccel < 0.0f) {
            p->climb = -p->climb;
        }
    }
}
