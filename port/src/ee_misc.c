/* Small EE kernel and SDK calls (still assembly in the decompilation) that
   the port answers itself. They define game names, so no <windows.h>. */
#include <stdint.h>
#include "grx_platform.h"
#include "grx_hwmem.h"

/* The GS's IMR, in the mapped privileged registers */
#define GS_IMR (*(volatile uint64_t *)(uintptr_t)(GRX_HW_GS_PRIV_BASE + 0x1010))

uint64_t GsGetIMR(void) {
    return GS_IMR;
}

uint64_t GsPutIMR(uint64_t imr) {
    uint64_t old = GS_IMR;
    GS_IMR = imr;
    return old;
}

/* The video mode sceGsResetGraph sets (NTSC 0x02, interlaced, field
   mode for this game). The GS takes what it shows from the display
   registers, so this is only kept for the record. */
int32_t grx_gs_crt_interlace, grx_gs_crt_mode, grx_gs_crt_ffmode;

void SetGsCrt(int32_t interlace, int32_t mode, int32_t ffmode) {
    grx_gs_crt_interlace = interlace;
    grx_gs_crt_mode = mode;
    grx_gs_crt_ffmode = ffmode;
}

/* DECI2, the debugger's channel: there is no debugger */
int32_t Deci2Call(int32_t call, void *arg) {
    (void)call;
    (void)arg;
    return -1;
}

/* The SDK's alarm patch (m:48CDA8) copies code into kernel memory and
   rewires syscalls. The port's kernel implements alarms itself
   (port/src/kernel.c), so these do nothing. */
int32_t __GetEntryAddress(int32_t syscall) {
    (void)syscall;
    return 0;
}

void __SetSyscall(int32_t syscall, int32_t entry) {
    (void)syscall;
    (void)entry;
}

void __kCopy(int32_t dst, void *src, int32_t size) {
    (void)dst;
    (void)src;
    (void)size;
}

/* fileio's SIF command handler (0x80000013): the IOP never sends it in
   the port */
void _sceFsCallbackHandler(void) {
}
