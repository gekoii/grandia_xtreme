/* Port-only C for EffSpawnByCode (0x005DE858), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "vu.h"
#include "gph.h"
#include "camera.h"
#include "ptcl.h"
#include "field/state.h"

void EffSpawnByCode(GphPart *part, s32 code, s32 mtx);

/* fldCamMovePos, the field camera's look-at */
#define CAM_POS (camField[0].lookAt)

/* m * (1, 1, 1, 1): the sum of the rows (the asm fills all four
   components with 1.0f before the transform) */
static void mtxOnes(f32 *dst, FVECTOR *m) {
    s32 i;

    for (i = 0; i < 4; i++) {
        dst[i] = m[0][i] * 1.0f + m[1][i] * 1.0f + m[2][i] * 1.0f + m[3][i] * 1.0f;
    }
}

/* Starts the map effect `code` (1-0x5C) on a model part with world
   matrix mtx and marks the part (flags |= 0x80). Some codes spawn at the
   origin with the matrix as the emission point, some at the transformed
   point and only within 6000 of the camera's look-at; codes 0x01/0x42/0x44
   are skipped on map 0x8010 and 0x28/0x43/0x47/0x48/0x59 on map 0x4810
   (without the mark). */
void EffSpawnByCode(GphPart *part, s32 code, s32 mtx) {
    FVECTOR v;
    FVECTOR org;
    FVECTOR *m = (FVECTOR *)mtx;
    PtclEmitter *e;
    f32 radius;

    code &= 0xFF;
    __builtin_memset(v, 0, 16);
    v[3] = 1.0f;
    __builtin_memset(org, 0, 16);
    org[3] = 1.0f;
    if (m == NULL || part == NULL) {
        return;
    }
    switch (code) {
    case 0x01:
    case 0x42:
    case 0x44:
        if (EvtGetMapNo() == 0x8010) {
            return;
        }
        e = (PtclEmitter *)EffSpawnCode01((s32)org);
        radius = 300.0f;
        goto light;
    case 0x02:
        e = (PtclEmitter *)EffSpawnCode02((s32)org);
        radius = 100.0f;
        goto light;
    case 0x03:
        e = (PtclEmitter *)EffSpawnCode03((s32)org);
        radius = 300.0f;
    light:
        if (e != NULL) {
            e->points = (FMATRIX *)m;
            FldPointLightAddFlicker(e->colorStart, mtx, radius);
        }
        break;
    case 0x04:
        mtxOnes(v, m);
        if (FldVecDist(v, CAM_POS) < 6000.0f) {
            e = (PtclEmitter *)EffSpawnCode04(0);
            goto points;
        }
        break;
    case 0x05:
    case 0x06:
    case 0x51:
        mtxOnes(v, m);
        if (FldVecDist(v, CAM_POS) < 6000.0f) {
            e = (PtclEmitter *)EffSpawnCode05(0);
            goto points;
        }
        break;
    case 0x11:
    case 0x13:
    case 0x41:
        e = (PtclEmitter *)EffSpawnCode11((s32)org);
        goto points;
    case 0x12:
        e = (PtclEmitter *)EffSpawnCode12((s32)org);
        goto points;
    case 0x14:
        mtxOnes(v, m);
        if (FldVecDist(v, CAM_POS) < 6000.0f) {
            e = (PtclEmitter *)EffSpawnCode14(0);
            goto points;
        }
        break;
    case 0x1E:
        mtxOnes(v, m);
        e = (PtclEmitter *)EffSpawnCode1E((s32)v);
        goto points;
    case 0x28:
    case 0x43:
    case 0x47:
    case 0x48:
        if (EvtGetMapNo() == 0x4810) {
            return;
        }
        mtxOnes(v, m);
        EffSpawnCode28((VuVec4 *)v);
        break;
    case 0x32:
    case 0x33:
        mtxOnes(v, m);
        if (FldVecDist(v, CAM_POS) < 6000.0f) {
            e = EffSpawnCode32(v, code - 0x32);
            goto points;
        }
        break;
    case 0x35:
        mtxOnes(v, m);
        EffSpawnCode35(v);
        break;
    case 0x36:
    case 0x5A:
        mtxOnes(v, m);
        EffSpawnCode5A((s32)v);
        break;
    case 0x3C:
        e = (PtclEmitter *)EffSpawnCode36((s32)org);
        if (e != NULL) {
            e->points = (FMATRIX *)m;
        }
        e = (PtclEmitter *)EffSpawnCode36((s32)org);
        goto points;
    case 0x45:
        e = (PtclEmitter *)EffSpawnCode45((s32)org);
        goto points;
    case 0x46:
        mtxOnes(v, m);
        EffSpawnCode46((s32)v);
        break;
    case 0x4F:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
        mtxOnes(v, m);
        if (FldVecDist(v, CAM_POS) < 6000.0f) {
            e = (PtclEmitter *)EffSpawnCode4F(0);
            goto points;
        }
        break;
    case 0x52:
        mtxOnes(v, m);
        if (FldVecDist(v, CAM_POS) < 6000.0f) {
            e = (PtclEmitter *)EffSpawnCode52(0);
            goto points;
        }
        break;
    case 0x59:
        if (EvtGetMapNo() == 0x4810) {
            return;
        }
        e = (PtclEmitter *)EffSpawnCode59((s32)org);
    points:
        if (e != NULL) {
            e->points = (FMATRIX *)m;
        }
        break;
    }
    part->flags |= 0x80;
}
