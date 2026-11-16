/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n29.h"
#include "gph.h"
#include "protos/battle.h"

void FxAfterimageUpdate(void) {
    FxAfterimage *p = &fxAfterimage;
    FxAfterimageSnap *q;
    GphModel *s;
    FxAfterimagePose *t;
    f32 f;

    if (p->unit != NULL) {
        if (p->count <= 0) {
            if (p->count == 0) {
                p->count = -1;
                p->frameStamp = btlFrameCount;
            } else if (p->frameStamp != btlFrameCount) {
                FxAfterimageFree();
            }
        } else {
            p->cur++;
            if (p->cur >= p->length) {
                p->cur = 0;
            }
            s = p->unit->model;
            t = &p->poses[p->cur];
            t->pos[0] = s->pos[0];
            t->rot[0] = s->rot[0];
            t->pos[1] = s->pos[1];
            t->rot[1] = s->rot[1];
            t->pos[2] = s->pos[2];
            t->rot[2] = s->rot[2];
            t->scale[0] = s->scale[0];
            t->motion = s->motion[0].motion;
            t->scale[1] = s->scale[1];
            t->scale[2] = s->scale[2];
            t->frame = s->motion[0].frame;
            if (p->full == 0) {
                if (!(p->cur & 1)) {
                    p->lag++;
                    if (p->lag >= p->length - 1) {
                        p->full = 1;
                    }
                }
            }
        }
    }
    q = &fxAfterimageSnap;
    if (q->motion != 0) {
        f = q->color[3] - 8.0f;
        q->color[3] = f;
        if (f < 0.0f) {
            q->color[3] = 0.0f;
            q->motion = 0;
        }
    }
}
