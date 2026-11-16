/* Port-only C for EvtAreaDebugDraw (0x005B3298), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "camera.h"
#include "field/state.h"
#include "field/event.h"

void VuMtxMul(void *, void *, void *);
void VuMtxStackLoad(void *);
void GphSphereDraw(s32, f32 *, u32);
void EvtAreaDebugDraw(EvtArea *e);

/* fldCamMovePos, the field camera's look-at */
#define CAM_POS (camField[0].lookAt)

/* Draws an event area's shape (debug view), skipping ones over 6000 from
   the camera's look-at; green/purple for walls, blue/red for the rest by
   EVT_AREA_ENABLED. The asm returns 0 (v0) on every path; the caller
   ignores it. */
void EvtAreaDebugDraw(EvtArea *e) {
    FVECTOR c;
    FVECTOR s;
    FMATRIX tm;
    FVECTOR l[2];
    FVECTOR l2[2];
    FMATRIX m;
    FMATRIX rm;
    FVECTOR v;
    FVECTOR w;
    f32 *cap;
    u32 col;
    s32 i;
    s32 j;

    if (e->shape == 1) {
        col = (e->flags & EVT_AREA_ENABLED) ? 0xFF00C000 : 0xFFC000C0;
    } else {
        col = (e->flags & EVT_AREA_ENABLED) ? 0xFF0000C0 : 0xFFC00000;
    }
    c[0] = 0.0f;
    c[1] = 0.0f;
    c[2] = 0.0f;
    if (e->link != NULL) {
        c[0] = e->link->pos[0];
        c[1] = e->link->pos[1];
        c[2] = e->link->pos[2];
    }
    c[0] = c[0] + e->pos[0];
    c[1] = c[1] + e->pos[1];
    c[2] = c[2] + e->pos[2];
    switch (e->shape) {
    case EVT_AREA_BOX:
        if (e->flags & 4) {
            VuMtxMul(tm, camField, e->mtx);
            VuMtxStackLoad(tm);
            FldDebugDrawBox(e->ofs, e->shapeData.box, col);
            VuMtxStackLoad(camField);
            return;
        }
        if (6000.0f < FldVecDist(c, CAM_POS)) {
            return;
        }
        if (e->flags & EVT_AREA_SCALED) {
            /* all three by scale[0] */
            s[0] = e->shapeData.box[0] * e->scale[0];
            s[1] = e->shapeData.box[1] * e->scale[0];
            s[2] = e->shapeData.box[2] * e->scale[0];
            FldDebugDrawBox(c, s, col);
        } else {
            FldDebugDrawBox(c, e->shapeData.box, col);
        }
        return;
    case EVT_AREA_CAPSULE:
        /* a wall: the segment a-b, box[8] high */
        cap = e->shapeData.box;
        for (i = 0; i < 2; i++) {
            if (e->flags & EVT_AREA_SCALED) {
                for (j = 0; j < 3; j++) {
                    l[i][j] = cap[i * 4 + j] * e->scale[j];
                }
                l[i][3] = cap[i * 4 + 3];
                for (j = 0; j < 3; j++) {
                    l[i][j] = l[i][j] + c[j];
                }
            } else {
                for (j = 0; j < 3; j++) {
                    l[i][j] = cap[i * 4 + j] + c[j];
                }
                l[i][3] = cap[i * 4 + 3];
            }
        }
        l[1][3] = 1.0f;
        l[0][3] = 1.0f;
        if (6000.0f < FldVecDist(l[0], CAM_POS) && 6000.0f < FldVecDist(l[1], CAM_POS)) {
            return;
        }
        GphLine3DDraw(2, l, col, 1);
        l[0][1] = l[0][1] + cap[8];
        l[1][1] = l[1][1] + cap[8];
        GphLine3DDraw(2, l, col, 1);
        l[0][1] = l[0][1] - cap[8];
        GphLine3DDraw(2, l, col, 1);
        l[1][1] = l[1][1] - cap[8];
        __builtin_memcpy(l2[0], l[0], 16);
        __builtin_memcpy(l2[1], l[0], 16);
        l2[1][1] = l2[1][1] + cap[8];
        GphLine3DDraw(2, l2, col, 1);
        __builtin_memcpy(l2[0], l[1], 16);
        __builtin_memcpy(l2[1], l[1], 16);
        l2[1][1] = l2[1][1] + cap[8];
        GphLine3DDraw(2, l2, col, 1);
        return;
    case EVT_AREA_CYLINDER:
        c[0] = e->shapeData.cylinder.ofs[0] + c[0];
        c[1] = e->shapeData.cylinder.ofs[1] + c[1];
        c[2] = e->shapeData.cylinder.ofs[2] + c[2];
        if (6000.0f < FldVecDist(c, CAM_POS)) {
            return;
        }
        if (e->flags & EVT_AREA_SCALED) {
            FldDebugDrawCylinder(c, col, e->shapeData.cylinder.radius * e->scale[0],
                                 e->shapeData.cylinder.height * e->scale[1]);
        } else {
            FldDebugDrawCylinder(c, col, e->shapeData.cylinder.radius, e->shapeData.cylinder.height);
        }
        return;
    case EVT_AREA_SPHERE:
        if (e->flags & 4) {
            /* The asm builds the translation from its sphere vector before
               filling it (stack garbage on the PS2); zero here */
            v[0] = 0.0f;
            v[1] = 0.0f;
            v[2] = 0.0f;
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    m[i][j] = (i == j) ? 1.0f : 0.0f;
                }
            }
            m[3][0] = v[0];
            m[3][1] = v[1];
            m[3][2] = v[2];
            VuMtxSetRotZYX((s32)rm, e->rot);
            VuMtxMul(m, m, rm);
            VuMtxMul(m, camField, m);
            VuMtxStackLoad(m);
            v[0] = e->ofs[0];
            v[1] = e->ofs[1];
            v[2] = e->ofs[2];
            v[3] = e->radius;
            GphSphereDraw(1, v, col);
            VuMtxStackLoad(camField);
        } else {
            w[0] = e->pos[0];
            w[1] = e->pos[1];
            w[2] = e->pos[2];
            w[3] = (e->flags & EVT_AREA_SCALED) ? e->radius * e->scale[0] : e->radius;
            GphSphereDraw(1, w, col);
        }
        return;
    }
}
