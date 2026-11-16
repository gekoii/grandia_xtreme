/* Port-only C for GphModelCalcPartsMotion (0x004349F8), which the decompilation still has in assembly. */
/*
 * Also GphModelCalcPartsMotionSplit (0x00434B30), GphModelCalcPartsMotionBlend
 * (0x00434C60) and GphModelCalcPartsMotionOverride (0x00434E30). Each walks
 * n parts: push a copy of the stack top, build the part's matrix on it
 * (its own T/R/S block when flags & 0x40, else from the motion), copy it to
 * part->mtx, pop part->unkA levels. The loops are do-whiles (n = 0 runs
 * once, as in the asm).
 */
#include "common.h"
#include "gph.h"
#include "protos/main.h"
#include "vu0_port.h"

/* Scratchpad matrices for the two motions of a blend */
#define SPR_BLEND_A ((f32 *)0x70002840)
#define SPR_BLEND_B ((f32 *)0x70002880)

void VuMtxStackPushCopy(void);
void VuMtxApplyTRS(s32 flags, f32 *m, f32 *trs);
void VuMtxBlend(f32 *dst, f32 *a, f32 *b, f32 wa, f32 wb);
void VuMtxNormalizeAxes(f32 *dst, f32 *src);
void VuMtxMul(f32 *dst, f32 *a, f32 *b);

void GphModelCalcPartsMotion(GphPart *part, s32 n, struct GphMotCursor *cur);
void GphModelCalcPartsMotionSplit(GphPart *part, s32 n, struct GphMotCursor *curA, struct GphMotCursor *curB);
void GphModelCalcPartsMotionBlend(GphPart *part, s32 n, struct GphMotCursor *curA, struct GphMotCursor *curB, f32 t);
void GphModelCalcPartsMotionOverride(GphPart *part, s32 n, struct GphMotCursor *cur);

/* A part with its own T/R/S (flags & 0x40): the low 3 flag bits switch
 * translate/rotate/scale off */
static void ApplyOwnTrs(GphPart *part, f32 *top) {
    u32 inv = ~(u32)part->flags;

    if (inv & 7) {
        VuMtxApplyTRS(inv, top, part->pos);
    }
}

/* drawFlags bit 0 (skinned): the part matrix is the identity */
static void StoreMatrix(GphPart *part, f32 *top, s32 skinnedUnit) {
    if (skinnedUnit && (part->drawFlags & 1)) {
        VuUnitMtx(part->mtx);
    } else {
        VuCopyMtx(part->mtx, top);
    }
}

static void PopLevels(GphPart *part) {
    if (part->unkA != 0) {
        VuMtxStackPopN(part->unkA);
    }
}

void GphModelCalcPartsMotion(GphPart *part, s32 n, struct GphMotCursor *cur) {
    do {
        f32 *top;

        VuMtxStackPushCopy();
        top = (f32 *)VuMtxStackGetTop();
        if (part->flags & 0x40) {
            ApplyOwnTrs(part, top);
        } else {
            GphMotPartCalcMatrix((s32)top, part, cur);
            GphMotCursorNextPart(cur);
        }
        StoreMatrix(part, top, 1);
        PopLevels(part);
        part++;
    } while (--n > 0);
}

/* Two motions: part->motionSlot picks the one this part follows; both
 * cursors advance */
void GphModelCalcPartsMotionSplit(GphPart *part, s32 n, struct GphMotCursor *curA, struct GphMotCursor *curB) {
    do {
        f32 *top;

        VuMtxStackPushCopy();
        top = (f32 *)VuMtxStackGetTop();
        if (part->flags & 0x40) {
            ApplyOwnTrs(part, top);
        } else {
            if (part->motionSlot == 0) {
                GphMotPartCalcMatrix((s32)top, part, curA);
            } else {
                GphMotPartCalcMatrix((s32)top, part, curB);
            }
            GphMotCursorNextPart(curA);
            GphMotCursorNextPart(curB);
        }
        StoreMatrix(part, top, 0);
        PopLevels(part);
        part++;
    } while (--n > 0);
}

/* Both motions evaluated on copies of the parent, blended (1 - t, t) and
 * re-orthonormalised */
void GphModelCalcPartsMotionBlend(GphPart *part, s32 n, struct GphMotCursor *curA, struct GphMotCursor *curB, f32 t) {
    do {
        f32 *top;

        VuMtxStackPushCopy();
        top = (f32 *)VuMtxStackGetTop();
        if (part->flags & 0x40) {
            ApplyOwnTrs(part, top);
        } else {
            VuCopyMtx(SPR_BLEND_A, top);
            VuCopyMtx(SPR_BLEND_B, top);
            GphMotPartCalcMatrix((s32)SPR_BLEND_A, part, curA);
            GphMotPartCalcMatrix((s32)SPR_BLEND_B, part, curB);
            GphMotCursorNextPart(curA);
            GphMotCursorNextPart(curB);
            VuMtxBlend(top, SPR_BLEND_A, SPR_BLEND_B, 1.0f - t, t);
            VuMtxNormalizeAxes(top, top);
        }
        StoreMatrix(part, top, 0);
        PopLevels(part);
        part++;
    } while (--n > 0);
}

/* flags & 0x100: the motion's matrix times the part's own matrix */
void GphModelCalcPartsMotionOverride(GphPart *part, s32 n, struct GphMotCursor *cur) {
    do {
        f32 *top;
        u16 flags;

        VuMtxStackPushCopy();
        top = (f32 *)VuMtxStackGetTop();
        flags = part->flags;
        if (flags & 0x40) {
            ApplyOwnTrs(part, top);
        } else {
            GphMotPartCalcMatrix((s32)top, part, cur);
            GphMotCursorNextPart(cur);
            if (flags & 0x100) {
                VuMtxMul(top, top, part->mtx);
            }
        }
        StoreMatrix(part, top, 1);
        PopLevels(part);
        part++;
    } while (--n > 0);
}
