/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccPartPairBackward(OccEntry *o) {
    OccPartPair *s = OCC_PARAMS(o, OccPartPair);
    GphPart *nodes[2];
    GphModel *e;
    s32 i;
    s32 j;

    e = FldObjGet(OCC_PARAMS(o, OccPartPair)->id);
    for (i = 0; i < 2; i++) {
        if (s->part[i] == 0) {
            nodes[i] = NULL;
        } else {
            nodes[i] = ChrModelFindPart(e, s->part[i]);
        }
        if (nodes[i] == NULL) {
            continue;
        }
        if (s->wait[i] > 0) {
            s->wait[i]--;
            continue;
        }
        if (s->done[i] >= s->count) {
            continue;
        }
        if (s->done[i] >= 0) {
            switch (s->mode) {
            case 0: {
                s32 v[3];
                v[0] = nodes[i]->rot[0];
                v[1] = nodes[i]->rot[1];
                v[2] = nodes[i]->rot[2];
                for (j = 0; j < 3; j++) {
                    if (s->step.i[j] != 0) {
                        if (i == 0) {
                            v[j] -= s->step.i[j];
                        } else {
                            v[j] += s->step.i[j];
                        }
                    }
                }
                nodes[i]->rot[0] = v[0];
                nodes[i]->rot[1] = v[1];
                nodes[i]->rot[2] = v[2];
                break;
            }
            case 1: {
                f32 v[3];
                v[0] = nodes[i]->pos[0];
                v[1] = nodes[i]->pos[1];
                v[2] = nodes[i]->pos[2];
                for (j = 0; j < 3; j++) {
                    if (s->step.f[j] != 0.0f) {
                        if (i == 0) {
                            v[j] -= s->step.f[j];
                        } else {
                            v[j] += s->step.f[j];
                        }
                    }
                }
                nodes[i]->pos[0] = v[0];
                nodes[i]->pos[1] = v[1];
                nodes[i]->pos[2] = v[2];
                break;
            }
            case 2: {
                f32 v[3];
                v[0] = nodes[i]->scale[0];
                v[1] = nodes[i]->scale[1];
                v[2] = nodes[i]->scale[2];
                for (j = 0; j < 3; j++) {
                    if (s->step.f[j] != 0.0f) {
                        v[j] -= s->step.f[j];
                    }
                }
                nodes[i]->scale[0] = v[0];
                nodes[i]->scale[1] = v[1];
                nodes[i]->scale[2] = v[2];
                break;
            }
            }
        }
        s->done[i]++;
    }
    if (s->done[0] == s->count && (nodes[1] == NULL || s->done[1] == s->done[0])) {
        o->timer = 0;
    } else {
        o->timer = s->count;
    }
}
