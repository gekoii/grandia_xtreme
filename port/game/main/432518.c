/* Port-only C for VuMtxStackLoad (0x00432518), which the decompilation still has in assembly. */
/*
 * VU0 registers: the asm copies the matrix through $vf1-$vf4. No caller
 * reads them afterwards (the inline asm after VuMtxStackLoad calls in
 * src/battle/68AFB0.c and 68CC80.c loads its own registers), so the port
 * doesn't write grx_vf.
 */
#include "common.h"
#include "main/n04.h"
#include "vu0_port.h"

void VuMtxStackLoad(FMATRIX *m);

/* Replaces the top of the stack with m */
void VuMtxStackLoad(FMATRIX *m) {
    VuCopyMtx(vuMtxStack->cur, m);
}
