/* Port-only C for EvtAreaCalcMatrix (0x005B2320), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/event.h"

void VuVecCopy(f32 *, f32 *);
void VuMtxSetRotX(FMATRIX *, s32);
void VuMtxMul(FVECTOR *, FMATRIX *, FMATRIX *);
void EvtAreaCalcMatrix(EvtArea *e);

/* mtx = translation to pos (plus the linked model's position) times the
   rotation by -rot[0] about X, then rot[1] about Y and rot[2] about Z */
void EvtAreaCalcMatrix(EvtArea *e) {
    FMATRIX trans;
    FMATRIX rot;
    FVECTOR pos;
    s32 i;
    s32 j;

    if (e->link != NULL) {
        pos[0] = e->pos[0] + e->link->pos[0];
        pos[1] = e->pos[1] + e->link->pos[1];
        pos[2] = e->pos[2] + e->link->pos[2];
        pos[3] = e->pos[3];
    } else {
        VuVecCopy(pos, e->pos);
    }
    /* vmr32 of vf0 builds the identity; the last row is vf0 + pos (xyz) */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            trans[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    trans[3][0] = pos[0];
    trans[3][1] = pos[1];
    trans[3][2] = pos[2];
    VuMtxSetRotX(&rot, -e->rot[0]);
    VuMtxRotY((s32)rot, e->rot[1], (s32)rot);
    VuMtxRotZ((s32)rot, e->rot[2], (s32)rot);
    VuMtxMul(e->mtx, &trans, &rot);
}
