/* Port-only C for isceSifSetDma (0x0048C430), which the decompilation still has in assembly. */
#include "common.h"
#include "sdk/sif.h"

/* Kernel call -0x77, iSifSetDma: the interrupt-context entry of syscall
   0x77 (sceSifSetDma, 0x0048C420). The port's kernel has one sceSifSetDma
   for both, so this forwards to it. */
u32 isceSifSetDma(sceSifDmaData *sdd, s32 len) {
    return sceSifSetDma(sdd, len);
}
