/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"


void ChrMotUpdate(GphModel *m, f32 f) {
    s32 fl;
    GphMotion *a;

    if (m != NULL) {
        fl = m->unk0;
        if (m->motionMode == 2) {
            f = 0.0f;
            a = m->motion[1].motion;
        } else {
            a = m->motion[0].motion;
        }
        if (a != NULL) {
            if (!(fl & 1)) {
                f = 0.0f;
            }
            if (fl & 2) {
                a->playFlags = 0;
            } else {
                a->playFlags = 1;
            }
            if (fl & 4) {
                a->playFlags = 0;
            } else {
                a->playFlags = 1;
            }
            if (fl & 8) {
                f = 0.0f;
            }
        }
        ChrModelUvScrollUpdate(m, 1.0f);
        if (f > 0.0f) {
            /* s64 cast: keeps jal + return instead of a tail call */
            ((s64 (*)(GphModel *, f32))ChrModelMotionUpdate)(m, f);
        }
    }
}
