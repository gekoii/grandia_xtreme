/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"


void ChrMotSetFrame(GphModel *m, f32 f) {
    if (m != NULL) {
        if ((f32)ChrMotGetLastFrame(m) < f) {
            f = ChrMotGetLastFrame(m);
        }
        if (m->motionMode == 2) {
            m->motion[1].frame = f;
        } else {
            m->motion[0].frame = f;
        }
    }
}
