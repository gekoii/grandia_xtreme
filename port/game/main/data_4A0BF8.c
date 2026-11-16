/* Port-only C for data the decompilation still has in assembly (asm/main/data/main/0A0BF8.data.s). */
#include "common.h"

/* src/main/48CDA8.c */
typedef struct {
    s32 a;
    s32 b;
} Pair;

extern const char _sceFsWildVersionStr[];
extern const char _sceSifLoadWildVersionStr[];
void _sceSioPutChar(s32);
s32 kCopy(s32 *, s32 *, u32);

u8 _sceLibkernelVersionNo[4] = "2340";

/* EE kernel code, copied to 0x80076000 and 0x82000 on the PS2. The
   port's __kCopy does nothing (port/src/ee_misc.c), so the code isn't
   carried here: the arrays keep only their sizes */
u8 _sceKernelAlarmPatch[1856];
u8 _sceKernelAlarmPatchStub[40];

/* kCopy's address cut to 32 bits isn't a constant on x86-64: set at
   start-up there (it fits: the image is below 2 GB, port/docs/win64.md) */
#if __SIZEOF_POINTER__ == 4
#define KCOPY_ADDR (s32)kCopy
#else
#define KCOPY_ADDR 0
static void __attribute__((constructor)) grx_alarm_syscall_table(void);
#endif
Pair _sceKernelAlarmSyscallTable[8] = {
    {0x5A, KCOPY_ADDR}, {0x5B, (s32)0x80076000}, {0xFC, 0}, {0xFE, 0},
    {0xFD, 0}, {0xFF, 0}, {0x12C, 0}, {8, 0},
};
#if __SIZEOF_POINTER__ != 4
static void grx_alarm_syscall_table(void) {
    _sceKernelAlarmSyscallTable[0].b = (s32)(__UINTPTR_TYPE__)kCopy;
}
#endif

void (*_scePrintfPutchar)(s32) = _sceSioPutChar;

volatile s32 _sceFsAsyncSemaTable[32] = {
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
};

s32 _sceFsCallSema = -1;

s32 _sceFsFdSema = -1;

s32 _sceFsAsyncSema = -1;

char *_sceFsWildVersion = (char *)_sceFsWildVersionStr;

s32 _sceSifLoadBindState = -1;

/* u8 * in src, then up to the next label: zeros */
struct {
    u8 *v;
    u8 tail[0x14];
} _sceSifLoadWildVersion = {
    (u8 *)_sceSifLoadWildVersionStr,
};
