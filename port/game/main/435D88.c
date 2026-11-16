/* Port-only C for GphModelCalcPartsMotionEx (0x00435D88), which the decompilation still has in assembly. */
/*
 * As GphModelCalcPartsMotion (4349F8.c) with GphMotPartCalcMatrixEx, the
 * push through VuMtxStackPush(NULL) and no identity for skinned parts.
 */
#include "common.h"
#include "gph.h"
#include "protos/main.h"
#include "vu0_port.h"

void VuMtxStackPush(FMATRIX *src);
void VuMtxApplyTRS(s32 flags, f32 *m, f32 *trs);

void GphModelCalcPartsMotionEx(GphPart *part, s32 n, struct GphMotCursor *cur);

void GphModelCalcPartsMotionEx(GphPart *part, s32 n, struct GphMotCursor *cur) {
    do {
        f32 *top;

        VuMtxStackPush(NULL);
        top = (f32 *)VuMtxStackGetTop();
        if (part->flags & 0x40) {
            u32 inv = ~(u32)part->flags;

            if (inv & 7) {
                VuMtxApplyTRS(inv, top, part->pos);
            }
        } else {
            GphMotPartCalcMatrixEx((s32)top, part, cur);
            GphMotCursorNextPart(cur);
        }
        VuCopyMtx(part->mtx, top);
        if (part->unkA != 0) {
            VuMtxStackPopN(part->unkA);
        }
        part++;
    } while (--n > 0);
}
