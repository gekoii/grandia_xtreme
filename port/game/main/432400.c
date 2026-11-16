/* Port-only C for VuMtxStackReset (0x00432400), which the decompilation still has in assembly. */
/*
 * Also VuMtxStackPush (0x00432430) and VuMtxStackPushCopy (0x00432480).
 *
 * VU0 registers: the asm builds the identity in $vf5-$vf7 and copies
 * through $vf1-$vf4; no caller reads them afterwards (checked: no inline
 * asm in src/ and no assembly reads a VF register after calling these), so
 * the port doesn't touch grx_vf.
 */
#include "common.h"
#include "main/n04.h"
#include "vu0_port.h"

void VuMtxStackReset(void);
void VuMtxStackPush(FMATRIX *src);
void VuMtxStackPushCopy(void);

/* Back to the bottom of the stack, which becomes the identity */
void VuMtxStackReset(void) {
    FMATRIX *m = vuMtxStack->base;

    vuMtxStack->cur = m;
    VuUnitMtx(&(*m)[0][0]);
}

/* Pushes a copy of src (NULL: of the top). Nothing happens when full. */
void VuMtxStackPush(FMATRIX *src) {
    FMATRIX *top = vuMtxStack->cur;
    FMATRIX *next = top + 1;

    if (src == NULL) {
        src = top;
    }
    if ((u32)next < (u32)vuMtxStack->end) {
        VuCopyMtx(next, src);
        vuMtxStack->cur = next;
    }
}

void VuMtxStackPushCopy(void) {
    FMATRIX *top = vuMtxStack->cur;
    FMATRIX *next = top + 1;

    if ((u32)next < (u32)vuMtxStack->end) {
        VuCopyMtx(next, top);
        vuMtxStack->cur = next;
    }
}
