#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n24.h"


void MiniConcertCarroStep(MiniConcertActor *c, u8 dir) {
    GphModel *m;
    GphModel *m2;
    f32 k;
    f32 k2;

    if (c->unk1A != 0) {
        m = c->chr->model;
        k = c->unk1A * 60.0f;
        k = k * cosf(c->chr->heading * 0.0000958738f);
        m->pos[0] += k;
        m2 = c->chr->model;
        k2 = c->unk1A * 60.0f;
        k2 = k2 * sinf(c->chr->heading * 0.0000958738f);
        m2->pos[2] += k2;
        c->unk1A = 0;
    }
    switch (dir) {
    case 19:
        if (c->unk19 == -1) {
            c->unk1A = 1;
            dir = 20;
            c->unk19 = 0;
        } else {
            c->unk1A = -1;
            c->unk19--;
        }
        break;
    case 20:
        if (c->unk19 == 1) {
            c->unk1A = -1;
            dir = 19;
            c->unk19 = 0;
        } else {
            c->unk1A = 1;
            c->unk19++;
        }
        break;
    }
    ChrMotSetImmediate(c->chr, dir);
    ChrMotFlagsSet(c->chr->model, 2, 0);
}
