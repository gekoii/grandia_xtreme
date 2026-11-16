/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "viewer/n35.h"

void VwrMotBlendStep(GphModel *p) {
    if (p != NULL && p->motionMode != 0) {
        p->blend += 1.0f / dbgMotObjParams[3].flag;
        if (p->blend >= 1.0f) {
            GphMotion *n = p->motion[1].motion;
            p->motion[1].motion = NULL;
            p->motion[0].frame = 0.0f;
            p->motionMode = 0;
            p->blend = 0.0f;
            p->motion[0].motion = n;
        }
    }
}
