/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/event.h"
#include "field/n13.h"
#include "protos/field.h"

void OccInterpVec(OccEntry *a, VuVec3 *out) {
    OccTween *m = OCC_PARAMS(a, OccTween);
    f32 t;

    if (a->state == 4) {
        m->count++;
        if (m->count >= m->time) {
            if (a->flags & OCC_REVERSE_END) {
                out->x = m->from[0];
                out->y = m->from[1];
                out->z = m->from[2];
            } else {
                out->x = m->to.f[0];
                out->y = m->to.f[1];
                out->z = m->to.f[2];
            }
            return;
        }
    } else if (a->state == 6) {
        m->count--;
        if (m->count <= 0) {
            out->x = m->from[0];
            out->y = m->from[1];
            out->z = m->from[2];
            return;
        }
    } else {
        return;
    }
    t = OccEase(m->ease - 1, (f32)m->count / (f32)m->time);
    out->x = m->from[0] + m->delta.f[0] * t;
    out->y = m->from[1] + m->delta.f[1] * t;
    out->z = m->from[2] + m->delta.f[2] * t;
}
