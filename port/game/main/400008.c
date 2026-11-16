/* Port-only C for _start (0x00400008), which the decompilation still has in assembly. */
/*
 * crt0 (asm/main/main/000000.s). The game's own "main" is SysMain
 * (src/main/4000C0.c, already C): _start calls it with the kernel's
 * argument block and nothing in the game is named main. This file defines
 * main, which port/include/grx_port.h renames GameMain, the function the
 * port's main() (port/src/main.c) calls.
 *
 * What _start does on the PS2, and where that went in the port:
 *   - clears from _fbss (ptySkillTable, 0x004A9480) up to D_750000 with sq:
 *     the main executable's .sbss/.bss and the load areas above it. In the
 *     port every one of those is a zero-initialised object of its own, which
 *     the Windows loader clears.
 *   - $gp = threadGp; syscall 0x3C SetupThread(threadGp, D_1FF0000, D_10000,
 *     _args, _root), sp = its result: the kernel makes the calling context
 *     the main thread (stack 0x01FF0000-0x02000000), fills _args from the
 *     boot arguments and makes _root (ExitThread) the return address of
 *     thread entries. In the port: the host thread is the main thread (the
 *     kernel layer has to adopt it as such), there is no $gp, and the
 *     argument block is filled below.
 *   - syscall 0x3D SetupHeap(D_750000, D_10000): newlib's sbrk area. newlib
 *     is the host C runtime in the port, so nothing.
 *   - FlushCache(0), ei: nothing to flush on the host; interrupts (the
 *     kernel layer's VBlank/DMA/GS handlers) may be delivered from here on.
 *   - SysMain(_args.argc, _args.argv), then Exit(its $v0). SysMain never
 *     returns (its frame loop is endless).
 * The game's heaps are not crt0's: SysMain itself calls
 * MemInit(7, 0x760000, 0x1FF0000) and MemHeapCreateAt(0x200000, ...) /
 * (0x300000, ...) with literal addresses, which the platform has to make
 * real memory before this runs (port/README.md, "Hardware addresses").
 */
#include "common.h"

/* The kernel's argument block (_args, src/main/bss_4A9680.c, 0x148 bytes) */
typedef struct {
    s32 argc;
    char *argv[16];
    char payload[0x100];
} CrtArgs;

extern u8 _args[];
void SysMain(s32 argc, char **argv);
void Exit(s32);

/* argv[0] as the PS2 BIOS gives it when it boots the disc; SysBoot keeps
   it in sysBootPath, shown only on the SYSTEM debug page */
static const char crtBootPath[] = "cdrom0:\\SLUS_204.17;1";

int main(int argc, char **argv) {
    CrtArgs *a = (CrtArgs *)_args;
    s32 i;

    /* The host's arguments (argv[0] is the port's "--boot") aren't the
       game's: it gets what the PS2 would give it */
    (void)argc;
    (void)argv;
    for (i = 0; crtBootPath[i] != 0 && i < (s32)sizeof(a->payload) - 1; i++) {
        a->payload[i] = crtBootPath[i];
    }
    a->payload[i] = 0;
    a->argc = 1;
    a->argv[0] = a->payload;

    /* SysMain takes a char **: the host's pointers, not the block's
       (whose argv is 32-bit EE addresses on x86-64, port/docs/win64.md) */
    {
        static char *host_argv[2];
        host_argv[0] = a->payload;
        SysMain(a->argc, host_argv);
    }
    Exit(0);
    return 0;
}
