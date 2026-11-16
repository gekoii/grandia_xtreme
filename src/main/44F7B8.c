#include "common.h"
#include "ptcl.h"
#include "protos/main.h"

void VuMtxStackPush(void *);
void PtclModelAlphaFade(GphModel *, PtclGenDef *, PtclModelPart *);
void VuMtxSetRotX(FMATRIX, s32);
void VuMtxSetRotY(FMATRIX, s32);
void VuMtxMul(s32, s32, FMATRIX);

void PtclDrawModels(s32 arg0)
{
    FVECTOR col;
    FVECTOR pos;
    s32 saved;
    PtclGen **pp;
    s32 n;
    PtclGenDef *f;
    s32 cnt;
    PtclModelPart *e;
    GphModel **ids;
    s32 a;

    pp = ptclCurSystem->gens;
    saved = GphDrawListGetCurrent();
    if (arg0 != 0) {
        GphDrawListSet(arg0);
    }
    for (n = ptclCurSystem->genCount; n > 0; n--, pp++) {
        if (*pp == NULL) {
            continue;
        }
        if ((*pp)->liveCount <= 0) {
            continue;
        }
        if ((*pp)->models == NULL) {
            continue;
        }
        f = &(*pp)->def;
        col[0] = f->color[0] - 128.0f;
        col[1] = f->color[1] - 128.0f;
        col[2] = f->color[2] - 128.0f;
        col[3] = f->color[3] - 128.0f;
        if ((*pp)->flags & 2) {
            if ((*pp)->flags & 0x800) {
                VuMtxSetRotX(ptclWorkMatrix, (*pp)->rot[0]);
                VuMtxRotY(ptclWorkMatrix, (*pp)->rot[1], ptclWorkMatrix);
                VuMtxRotZ(ptclWorkMatrix, (*pp)->rot[2], ptclWorkMatrix);
            } else {
                VuMtxSetRotY(ptclWorkMatrix, (*pp)->rot[2]);
                VuMtxRotX(ptclWorkMatrix, (*pp)->rot[0], ptclWorkMatrix);
                VuMtxRotY(ptclWorkMatrix, (*pp)->rot[1], ptclWorkMatrix);
            }
            __asm__ volatile(
                "lqc2 $vf1, 0x0(%2)\n\t"
                "lqc2 $vf4, 0x0(%1)\n\t"
                "lqc2 $vf5, 0x10(%1)\n\t"
                "lqc2 $vf6, 0x20(%1)\n\t"
                "lqc2 $vf7, 0x30(%1)\n\t"
                "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
                "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
                "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
                "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
                "sqc2 $vf1, 0x0(%0)"
                : : "r"(pos), "r"(ptclWorkMatrix), "r"(f->pos) : "memory");
            if ((*pp)->flags & 0x800) {
                VuMtxSetRotX(ptclWorkMatrix, f->rot[0] + (*pp)->rot[0]);
                VuMtxRotY(ptclWorkMatrix, f->rot[1] + (*pp)->rot[1], ptclWorkMatrix);
                VuMtxRotZ(ptclWorkMatrix, f->rot[2] + (*pp)->rot[2], ptclWorkMatrix);
            } else {
                VuMtxSetRotY(ptclWorkMatrix, f->rot[2] + (*pp)->rot[2]);
                VuMtxRotX(ptclWorkMatrix, f->rot[0] + (*pp)->rot[0], ptclWorkMatrix);
                VuMtxRotY(ptclWorkMatrix, f->rot[1] + (*pp)->rot[1], ptclWorkMatrix);
            }
            __asm__ volatile(
                "lqc2 $vf1, 0x0(%0)\n\t"
                "lqc2 $vf2, 0x0(%1)\n\t"
                "vadd.xyz $vf1, $vf1, $vf2\n\t"
                "sqc2 $vf1, 0x0(%0)"
                : : "r"(pos), "r"((*pp)->pos) : "memory");
            /* The original passes pos[0], a float, as the vector's address. */
            __asm__ volatile(
                "lqc2 $vf1, 0x30(%1)\n\t"
                "lqc2 $vf2, 0x0(%2)\n\t"
                "lqc2 $vf3, 0x0(%1)\n\t"
                "lqc2 $vf4, 0x10(%1)\n\t"
                "lqc2 $vf5, 0x20(%1)\n\t"
                "vadd.xyz $vf1, $vf1, $vf2\n\t"
                "sqc2 $vf3, 0x0(%0)\n\t"
                "sqc2 $vf4, 0x10(%0)\n\t"
                "sqc2 $vf5, 0x20(%0)\n\t"
                "sqc2 $vf1, 0x30(%0)"
                : : "r"(ptclWorkMatrix), "r"(ptclWorkMatrix), "r"(pos[0]) : "memory");
            VuMtxStackPush(NULL);
            a = VuMtxStackGetTop();
            VuMtxMul(a, VuMtxStackGetTop(), ptclWorkMatrix);
        }
        cnt = (*pp)->groupCount;
        e = (*pp)->modelParts;
        ids = (*pp)->models;
        for (; cnt > 0; cnt--, e++, ids++) {
            if (e->life != 0) {
                ChrModelSetColor(*ids, col);
                PtclModelAlphaFade(*ids, f, e);
                GphModelCalcAndDraw(*ids);
            }
        }
        if ((*pp)->flags & 2) {
            VuMtxStackPop();
        }
    }
    GphDrawListSet(saved);
}
