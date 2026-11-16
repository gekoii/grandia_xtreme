/* rodata: 0x004A6910-0x004A6928 */
#include "common.h"
#include "protos/main.h"
#include "ptcl.h"




void VuMtxSetRotX(void *, s32);
void VuMtxSetRotY(void *, s32);
void VuMtxMul(void *, s32, void *);

extern u8 ptclRsAlphaFog[];
extern u8 ptclRsAddFog[];
extern u8 ptclRsSubFog[];
extern u8 ptclRsAlpha[];
extern u8 ptclRsAdd[];
extern u8 ptclRsSub[];

#define APPLY(out, m, v) __asm__ volatile( \
    "lqc2 $vf1, 0(%2)\n\t" \
    "lqc2 $vf4, 0x0(%1)\n\t" \
    "lqc2 $vf5, 0x10(%1)\n\t" \
    "lqc2 $vf6, 0x20(%1)\n\t" \
    "lqc2 $vf7, 0x30(%1)\n\t" \
    "vmulax.xyzw ACC, $vf4, $vf1x\n\t" \
    "vmadday.xyzw ACC, $vf5, $vf1y\n\t" \
    "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t" \
    "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(out), "r"(m), "r"(v) : "memory")

#define ADD_XYZ(d, a, b) __asm__ volatile( \
    "lqc2 $vf1, 0(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "vadd.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf1, 0(%0)" \
    : : "r"(d), "r"(a), "r"(b) : "memory")

#define TRANS(d, m, t) __asm__ volatile( \
    "lqc2 $vf1, 0x30(%1)\n\t" \
    "lqc2 $vf2, 0(%2)\n\t" \
    "lqc2 $vf3, 0x0(%1)\n\t" \
    "lqc2 $vf4, 0x10(%1)\n\t" \
    "lqc2 $vf5, 0x20(%1)\n\t" \
    "vadd.xyz $vf1, $vf1, $vf2\n\t" \
    "sqc2 $vf3, 0x0(%0)\n\t" \
    "sqc2 $vf4, 0x10(%0)\n\t" \
    "sqc2 $vf5, 0x20(%0)\n\t" \
    "sqc2 $vf1, 0x30(%0)" \
    : : "r"(d), "r"(m), "r"(t) : "memory")

void PtclDrawSprites(s32 arg0)
{
    FVECTOR v;
    u8 cur;
    s32 n;
    PtclGen **list;
    PtclGenDef *sub;
    void *mtx;
    s32 i;
    s32 *cnt;
    s32 a;
    s32 b;
    s32 c;

    if (arg0 != 0) {
        s32 old = GphDrawListGetCurrent();
        GphDrawListSet(arg0);
        GphDrawListDrawAll();
        GphDrawListSet(old);
    }
    cur = 0;
    n = ptclCurSystem->genCount;
    for (list = ptclCurSystem->gens; n > 0; n--, list++) {
        if (*list == NULL || (*list)->liveCount <= 0 || (*list)->models != 0) {
            continue;
        }
        switch ((*list)->blendMode) {
        case 0:
            GsRenderStatePacketSend(ptclRsAlpha);
            break;
        case 1:
            GsRenderStatePacketSend(ptclRsAdd);
            break;
        case 2:
            GsRenderStatePacketSend(ptclRsSub);
            break;
        case 4:
            GsRenderStatePacketSend(ptclRsAddFog);
            break;
        case 5:
            GsRenderStatePacketSend(ptclRsSubFog);
            break;
        default:
            GsRenderStatePacketSend(ptclRsAlphaFog);
            break;
        }
        if (cur != (*list)->texId) {
            cur = (*list)->texId;
            GphTexBind(cur);
        }
        if ((*list)->colorPkt != 0) {
            PtclSpriteColorAnim(*list);
        }
        if ((*list)->animSet != 0) {
            PtclSpriteSetAnimUvs(*list);
        }
        sub = &(*list)->def;
        if ((*list)->flags & 2) {
            if ((*list)->flags & 0x800) {
                VuMtxSetRotX(ptclWorkMatrix, (*list)->rot[0]);
                VuMtxRotY(ptclWorkMatrix, (*list)->rot[1], ptclWorkMatrix);
                VuMtxRotZ(ptclWorkMatrix, (*list)->rot[2], ptclWorkMatrix);
            } else {
                VuMtxSetRotY(ptclWorkMatrix, (*list)->rot[2]);
                VuMtxRotX(ptclWorkMatrix, (*list)->rot[0], ptclWorkMatrix);
                VuMtxRotY(ptclWorkMatrix, (*list)->rot[1], ptclWorkMatrix);
            }
            APPLY(v, ptclWorkMatrix, sub->pos);
            if ((*list)->flags & 0x800) {
                VuMtxSetRotX(ptclWorkMatrix, sub->rot[0] + (*list)->rot[0]);
                VuMtxRotY(ptclWorkMatrix, sub->rot[1] + (*list)->rot[1], ptclWorkMatrix);
                VuMtxRotZ(ptclWorkMatrix, sub->rot[2] + (*list)->rot[2], ptclWorkMatrix);
            } else {
                VuMtxSetRotY(ptclWorkMatrix, sub->rot[2] + (*list)->rot[2]);
                VuMtxRotX(ptclWorkMatrix, sub->rot[0] + (*list)->rot[0], ptclWorkMatrix);
                VuMtxRotY(ptclWorkMatrix, sub->rot[1] + (*list)->rot[1], ptclWorkMatrix);
            }
            ADD_XYZ(v, v, (*list)->pos);
            TRANS(ptclWorkMatrix, ptclWorkMatrix, v);
            VuMtxMul(ptclWorkMatrix, VuMtxStackGetTop(), ptclWorkMatrix);
            mtx = ptclWorkMatrix;
        } else {
            mtx = (void *)VuMtxStackGetTop();
        }
        c = (s32)(*list)->uvPkt;
        a = (s32)(*list)->sprites;
        cnt = (*list)->groupLive;
        b = (s32)(*list)->colorPkt;
        for (i = (*list)->groupCount; i > 0; i--, cnt++, a += 0x7C0) {
            if (*cnt > 0) {
                PtclDrawStrip((s32)mtx, a, b, c, sub->color);
            }
            if (b != 0) {
                b += 0x200;
            }
            if (c != 0) {
                c += 0x200;
            }
        }
    }
}
