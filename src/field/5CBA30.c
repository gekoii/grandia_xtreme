/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "protos/field.h"
#include "field/state.h"

void VuVecCopy(void *, void *);

void CamAiTown(Camera *o, ChrObj *s, void *pad) {
    f32 v;
    f32 t;

    VuVecCopy(o->lookAt, s->model->pos);
    o->mode = CAM_MODE_NORMAL;
    o->lookAt[1] += camCtl.height;
    o->dist = 1800.0f;
    o->fov = 25.0f;
    if (FldFlagListIsOpen() == 0 && EvtScnrIsRunning() == 0) {
        o->yaw = CamPadTurnGet(o, pad, 0, 0.93f, o->yaw * 57.295776f) * 0.017453292f;
    }
    o->pitch = 0.34906584f;
    if (s->speed < 11.99f) {
        if (camCtl.unkA < 10) {
            camCtl.unkA++;
            v = o->zoom;
            if (v < 1.0f) {
                o->zoom = v + (1.0f - v) / 10.0f;
                v = o->zoom;
            }
        } else {
            v = o->zoom;
            if (v < 1.0f) {
                t = v + (1.0f - v) / 10.0f;
                o->zoom = t;
                v = t;
            }
        }
        if (v > 1.0f) {
            t = v - (v - 1.0f) / 10.0f;
            o->zoom = t;
            if (t < 1.0f) {
                o->zoom = 1.0f;
            }
        }
    } else {
        camCtl.unkA = 0;
        v = o->zoom;
        if (v >= 0.85f) {
            t = v - (v - 0.85f) / 50.0f;
            o->zoom = t;
            if (t < 0.85f) {
                o->zoom = 0.85f;
            }
        }
    }
}
