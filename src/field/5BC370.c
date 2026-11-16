#include "common.h"
#include "protos/field.h"
#include "field/event.h"
#include "field/n13.h"


void OccShakeForward(OccEntry *o) {
    OccShake *s = OCC_PARAMS(o, OccShake);
    GphModel *e;
    GphPart *n;
    s32 i;
    s32 t;
    s32 d;
    s32 r;

    e = FldObjGet(OCC_PARAMS(o, OccShake)->id);
    if (s->part != 0) {
        n = ChrModelFindPart(e, s->part);
    } else {
        n = NULL;
    }
    for (i = 0; i < 3; i++) {
        if (s->wait[i] != 0) {
            s->wait[i]--;
            continue;
        }
        if (s->dir[i] == 0) {
            continue;
        }
        t = s->rot[i] + s->speed[i];
        s->rot[i] = t;
        if (n != NULL) {
            n->rot[i] = t;
        } else {
            e->rot[i] = t;
        }
        if (s->dir[i] > 0) {
            t = s->speed[i];
            r = t - s->accel;
            if (r <= 0 && t > 0) {
                s->speed[i] = -t;
            } else {
                s->speed[i] = r;
            }
            if (s->base[i] < s->rot[i]) {
                continue;
            }
            d = s->swing[i] - s->decay;
            s->swing[i] = d;
            if (s->decay < d) {
                s->speed[i] = -d;
                s->dir[i] = -1;
            } else {
                s->dir[i] = 0;
            }
        } else if (s->dir[i] < 0) {
            t = s->speed[i];
            r = t + s->accel;
            if (r >= 0 && t < 0) {
                s->speed[i] = -t;
            } else {
                s->speed[i] = r;
            }
            if (s->rot[i] < s->base[i]) {
                continue;
            }
            d = s->swing[i] - s->decay;
            s->swing[i] = d;
            if (s->decay < d) {
                s->speed[i] = d;
                s->dir[i] = 1;
            } else {
                s->dir[i] = 0;
            }
        }
    }
    if (s->dir[0] == 0 && s->dir[1] == 0 && s->dir[2] == 0) {
        o->timer = 0;
    } else {
        o->timer = 1;
    }
}
