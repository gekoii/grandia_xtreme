/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"

typedef struct { f32 x, y, z, w; } V4;
typedef struct { V4 v[2]; f32 x20; } Cam;

extern Cam chrLadderWork;


void ChrLadderStart(f32 *p, f32 f) {
    ChrObj *c;
    s32 i;
    s32 n;
    f32 ref;
    f32 y1;
    double d0;
    double d1;

    c = ChrGetByNo(1);
    if (c->ctrlMode == 2) {
        return;
    }
    if (p[1] > p[4]) {
        chrLadderWork.v[0].x = p[3];
        chrLadderWork.v[0].y = p[4];
        chrLadderWork.v[0].z = p[5];
        chrLadderWork.v[1].x = p[0];
        chrLadderWork.v[1].y = p[1];
        chrLadderWork.v[1].z = p[2];
    } else {
        for (i = 0; i < 2; i++) {
            chrLadderWork.v[i].x = p[i * 3 + 0];
            chrLadderWork.v[i].y = p[i * 3 + 1];
            chrLadderWork.v[i].z = p[i * 3 + 2];
        }
    }
    chrLadderWork.x20 = f;
    ref = c->model->pos[1];
    y1 = chrLadderWork.v[1].y;
    d0 = chrLadderWork.v[0].y - ref;
    if (d0 < 0.0) {
        d0 = 0.0 - d0;
    }
    d1 = y1 - ref;
    if (d1 < 0.0) {
        d1 = 0.0 - d1;
    }
    n = d0 < d1 ? 30 : 40;
    c->ladderState = 0;
    c->ctrlMode = 2;
    c->unk28 = n;
    FldInputLockSet(1);
}
