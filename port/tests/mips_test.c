/*
 * Tests for the MIPS interpreter (port/src/mips/, port/include/grx_mips.h).
 *
 *   clang -target i686-w64-mingw32 -std=gnu11 -Wall -O1 -msse2 -mfpmath=sse \
 *       -Iport/include -Iinclude port/tests/mips_test.c port/src/mips/cpu.c \
 *       port/src/mips/thunks.c port/src/mips/builtins.c port/src/mips/disasm.c \
 *       port/src/mips/abi_gen.c port/src/ee.c -Wl,--image-base=0x04000000 \
 *       -Wl,--disable-dynamicbase -o mips_test.exe
 *   mips_test [iso dir] [call log]      (default: iso, no log; e.g.
 *                                        build/port/mips_calls.txt)
 *
 * The fixed image base keeps the test's own memory out of the EE range the
 * interpreter reserves for the ABI globals (0x400000-0x760000), as the
 * port's does. MIPS_TEST_TRACE=1 names each program as it starts,
 * MIPS_TEST_CALLS=1 also logs every bridged call to stderr.
 *
 * 1. Instruction semantics on hand-assembled code: delay slots, branch
 *    likely, the R5900's mult rd and division by zero, unaligned ldl/ldr/
 *    sdl/sdr, the EE FPU's edge cases, bridged calls with int and float
 *    arguments, ABI globals, the EE/host forms of addresses, thunks,
 *    nested calls.
 * 2. Every instruction kind the disc's programs contain (all 486, scanned)
 *    once in a sandbox: none may be unknown to the interpreter.
 * 3. Every program run from its entry, then its hooks (the BtlCharDef and
 *    MgcWork function pointers it set, the callbacks it handed to the
 *    bridge) for a few frames, under a fake bridge that logs each call and
 *    returns empty objects, on empty game state (null pointers read a
 *    zero page). A program stopping on an unknown instruction, a missing
 *    bridge function or execution outside its code is a failure; stopping
 *    because the empty state isn't what it expects is counted, not failed.
 *
 * The programs are read from the extracted disc at run time; nothing of
 * them is printed but addresses, names and counts. Exit status: failures.
 */
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_ee.h"
#include "grx_mips.h"

/* ------------------------------------------------------------------ platform */

static jmp_buf *trap;
static char fatal_msg[512];

void grx_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(fatal_msg, sizeof(fatal_msg), fmt, ap);
    va_end(ap);
    if (trap) {
        longjmp(*trap, 1);
    }
    fprintf(stderr, "fatal: %s\n", fatal_msg);
    exit(100);
}

static int quiet_logs;
static char last_log[512];

void grx_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(last_log, sizeof(last_log), fmt, ap);
    va_end(ap);
    /* the interpreter's notes about the bridge are always shown */
    if (!quiet_logs || strstr(last_log, "as a pointer") || strstr(last_log, "native function")) {
        fprintf(stderr, "log: %s\n", last_log);
    }
}

static int failures;

#define CHECK(cond, ...) do { \
        if (!(cond)) { \
            failures++; \
            printf("FAIL %s:%d: ", __func__, __LINE__); \
            printf(__VA_ARGS__); \
            printf("\n"); \
        } \
    } while (0)

/* ------------------------------------------------------------------ memory */

#define EE_DATA_LO 0x004A0000u
#define EE_DATA_HI 0x00700000u

static uint8_t window[GRX_MIPS_WIN_SIZE] __attribute__((aligned(16)));
static uint8_t mstack[0x40000] __attribute__((aligned(16)));
static uint8_t null_page[0x10010] __attribute__((aligned(16)));
/* The ABI globals, laid out as in EE memory (host = data_arena + ee - EE_DATA_LO) */
static uint8_t data_arena[EE_DATA_HI - EE_DATA_LO] __attribute__((aligned(16)));
static void *data_hosts[4096];
/* Objects the fake bridge hands out */
static uint8_t heap[16 << 20] __attribute__((aligned(16)));
static uint32_t heap_used;
static uint8_t file[2 << 20] __attribute__((aligned(16)));
static uint32_t file_size;
static uint8_t mgc_work[0xCA0 + 0x100] __attribute__((aligned(16)));
static uint8_t scratch[0x10000] __attribute__((aligned(16)));

/* Is a host address one of the test's buffers? */
static int host_ok(uint32_t a) {
    static const struct { const void *p; uint32_t n; } r[] = {
        {data_arena, sizeof(data_arena)}, {heap, sizeof(heap)}, {file, sizeof(file)}, {mgc_work, sizeof(mgc_work)},
        {scratch, sizeof(scratch)}, {mstack, sizeof(mstack)}, {window, sizeof(window)}, {null_page, sizeof(null_page)},
    };
    unsigned i;
    for (i = 0; i < sizeof(r) / sizeof(r[0]); i++) {
        if (a - (uint32_t)(uintptr_t)r[i].p < r[i].n) {
            return 1;
        }
    }
    return 0;
}

static void *host_of(uint32_t ee) {
    return data_arena + (ee - EE_DATA_LO);
}

static void *heap_alloc(uint32_t n) {
    void *p;
    n = (n + 15) & ~15u;
    if (n > 0x10000) {
        n = 0x10000;
    }
    if (heap_used + n > sizeof(heap)) {
        heap_used = 0;
    }
    p = heap + heap_used;
    memset(p, 0, n);
    heap_used += n;
    return p;
}

static const GrxMipsAbiData *abi_data(const char *name) {
    int i;
    for (i = 0; i < grx_mips_abi_ndata; i++) {
        if (strcmp(grx_mips_abi_data[i].name, name) == 0) {
            return &grx_mips_abi_data[i];
        }
    }
    printf("no ABI global %s\n", name);
    exit(1);
}

static uint32_t abi_func(const char *name) {
    int i;
    for (i = 0; i < grx_mips_abi_nfuncs; i++) {
        if (strcmp(grx_mips_abi_funcs[i].name, name) == 0) {
            return grx_mips_abi_funcs[i].addr;
        }
    }
    printf("no ABI function %s\n", name);
    exit(1);
}

/* ------------------------------------------------------------------ fake bridge */

/* The callbacks programs hand to native code (thunks), to run later */
#define MAXCB 64
static void *callbacks[MAXCB];
static const char *callback_from[MAXCB];
static int ncallbacks;

static void note_callback(void *p, const char *from) {
    int i;
    if (!grx_mips_thunk_target(p)) {
        return;
    }
    for (i = 0; i < ncallbacks; i++) {
        if (callbacks[i] == p) {
            return;
        }
    }
    if (ncallbacks < MAXCB) {
        callback_from[ncallbacks] = from;
        callbacks[ncallbacks++] = p;
    }
}

/* Call log of the current program */
static char calllog[1 << 16];
static size_t calllog_len;
static int ncalls;
static int call_counts[1024];
static int first_calls[1024];
static int test_hook; /* unit tests: what the handler records */
static uint32_t rec_i[8];
static float rec_f[8];
static const char *rec_name;
static void *(*rec_callback)(void);

static void logf_(const char *fmt, ...) {
    va_list ap;
    if (calllog_len + 200 >= sizeof(calllog)) {
        return;
    }
    va_start(ap, fmt);
    calllog_len += (size_t)vsnprintf(calllog + calllog_len, sizeof(calllog) - calllog_len, fmt, ap);
    va_end(ap);
}

static uint32_t rng = 12345;

static void fake(GrxMipsCpu *c, const GrxMipsAbiFunc *f) {
    const char *s = strchr(f->sig, ':') + 1;
    char ret = f->sig[0];
    int ni = 0, nf = 0, k = (int)(f - grx_mips_abi_funcs);
    uint32_t iv[8] = {0};
    float fv[8] = {0};
    void *pv[8] = {0};
    if (ncalls == 0 && k >= 0 && k < 1024) {
        first_calls[k]++;
    }
    ncalls++;
    if (k >= 0 && k < 1024) {
        call_counts[k]++;
    }
    logf_("  %s(", f->name);
    for (; *s; s++) {
        if (*s == '.') {
            logf_("...");
            break;
        }
        if (*s == 'f') {
            fv[nf] = grx_mips_arg_f(c, nf);
            logf_("%s%g", ni + nf ? ", " : "", fv[nf]);
            nf++;
        } else if (*s == 'p') {
            pv[ni] = grx_mips_arg_p(c, ni);
            iv[ni] = (uint32_t)(uintptr_t)pv[ni];
            note_callback(pv[ni], f->name);
            if (grx_mips_thunk_target(pv[ni])) {
                logf_("%sthunk(%08X)", ni + nf ? ", " : "", grx_mips_thunk_target(pv[ni]));
            } else {
                logf_("%s@%08X", ni + nf ? ", " : "", iv[ni]);
            }
            ni++;
        } else {
            iv[ni] = (uint32_t)grx_mips_arg_i(c, ni);
            logf_("%s%d", ni + nf ? ", " : "", (int32_t)iv[ni]);
            ni++;
        }
    }
    logf_(")\n");
    if (test_hook) {
        memcpy(rec_i, iv, sizeof(rec_i));
        memcpy(rec_f, fv, sizeof(rec_f));
        rec_name = f->name;
    }
    /* A few that do what they say, so the programs get somewhere */
    if (strcmp(f->name, "BtlPakGetEntry") == 0) {
        uint8_t *pak = pv[0];
        uint32_t n = iv[1];
        if (pak >= file && pak < file + file_size) {
            uint32_t count, off;
            memcpy(&count, pak, 4);
            if (n < count) {
                memcpy(&off, pak + 4 + 8 * n, 4);
                grx_mips_ret_p(c, off ? pak + off : 0);
                return;
            }
        }
        grx_mips_ret_p(c, heap_alloc(0x1000));
        return;
    }
    if (strcmp(f->name, "memset") == 0 || strcmp(f->name, "MemSet") == 0) {
        if (iv[2] < 0x10000 && host_ok(iv[0]) && host_ok(iv[0] + iv[2])) {
            memset(pv[0], (int)iv[1], iv[2]);
        }
        if (ret == 'p') {
            grx_mips_ret_p(c, pv[0]);
        }
        return;
    }
    if (strcmp(f->name, "rand") == 0) {
        rng = rng * 1103515245u + 12345u;
        grx_mips_ret_i(c, (int32_t)((rng >> 1) & 0x7FFFFFFF));
        return;
    }
    if (strcmp(f->name, "BtlFileLoadAsync") == 0 && !pv[0]) {
        grx_mips_ret_p(c, file); /* the program's own pack, as if just loaded */
        return;
    }
    if (strcmp(f->name, "BtlFileRelocate") == 0) {
        grx_mips_ret_p(c, pv[0] ? (uint8_t *)pv[0] + 0x10 : 0);
        return;
    }
    if (strcmp(f->name, "MgcRelocateForUnit") == 0) {
        grx_mips_ret_p(c, pv[0]);
        return;
    }
    if (strcmp(f->name, "BtlPartyFindUnit") == 0 || strcmp(f->name, "BtlEnemyFindUnit") == 0) {
        uint32_t i = (f->name[3] == 'P' ? iv[0] : 4 + iv[0]) & 15;
        grx_mips_ret_p(c, (uint8_t *)host_of(abi_data("btlUnits")->ee) + 0x200 * i);
        return;
    }
    if (strcmp(f->name, "VuSinCosAngle") == 0) {
        float a = (float)(int32_t)iv[2] * (6.2831853f / 65536.0f);
        if (host_ok(iv[0])) {
            *(float *)pv[0] = sinf(a);
        }
        if (host_ok(iv[1])) {
            *(float *)pv[1] = cosf(a);
        }
        return;
    }
    if (strcmp(f->name, "MgcSetNextProc") == 0 && test_hook && rec_callback) {
        /* unit test: call the callback right away (a nested call) */
        int (*fn)(int) = (int (*)(int))pv[0];
        rec_i[7] = (uint32_t)fn(41);
        return;
    }
    switch (ret) {
    case 'p': grx_mips_ret_p(c, heap_alloc(0x1000)); break;
    case 'f': grx_mips_ret_f(c, 0.0f); break;
    case 'd': grx_mips_ret_d(c, 0.0); break;
    case 'l': grx_mips_ret_l(c, 0); break;
    case 'v': break;
    default: grx_mips_ret_i(c, 0); break;
    }
}

static GrxMipsHandler handlers[1024];
/* Stand-ins for the port's functions: distinct addresses the bridge maps
   back to its handlers (never called natively) */
static char fake_fns[1024];
static void *func_hosts[1024];

/* ------------------------------------------------------------------ assembler */

enum { ZERO, AT, V0, V1, A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, T6, T7, S0, S1, S2, S3, SP = 29, RA = 31 };
#define R_(rs, rt, rd, sa, fn) ((uint32_t)(rs) << 21 | (uint32_t)(rt) << 16 | (uint32_t)(rd) << 11 | (uint32_t)(sa) << 6 | (fn))
#define I_(op, rs, rt, imm) ((uint32_t)(op) << 26 | (uint32_t)(rs) << 21 | (uint32_t)(rt) << 16 | ((uint32_t)(imm) & 0xFFFF))
#define NOP 0u
#define ADDIU(rt, rs, imm) I_(0x09, rs, rt, imm)
#define ORI(rt, rs, imm) I_(0x0D, rs, rt, imm)
#define LUI(rt, imm) I_(0x0F, 0, rt, imm)
#define LW(rt, off, b) I_(0x23, b, rt, off)
#define SW(rt, off, b) I_(0x2B, b, rt, off)
#define LD(rt, off, b) I_(0x37, b, rt, off)
#define SD(rt, off, b) I_(0x3F, b, rt, off)
#define LDL(rt, off, b) I_(0x1A, b, rt, off)
#define LDR(rt, off, b) I_(0x1B, b, rt, off)
#define SDL(rt, off, b) I_(0x2C, b, rt, off)
#define SDR(rt, off, b) I_(0x2D, b, rt, off)
#define BEQ(rs, rt, off) I_(0x04, rs, rt, off)
#define BEQL(rs, rt, off) I_(0x14, rs, rt, off)
#define JR(rs) R_(rs, 0, 0, 0, 0x08)
#define JALR(rd, rs) R_(rs, 0, rd, 0, 0x09)
#define JAL(t) ((uint32_t)3 << 26 | (((t) >> 2) & 0x03FFFFFF))
#define ADDU(rd, rs, rt) R_(rs, rt, rd, 0, 0x21)
#define DADDU(rd, rs, rt) R_(rs, rt, rd, 0, 0x2D)
#define MULT(rd, rs, rt) R_(rs, rt, rd, 0, 0x18)
#define DIV(rs, rt) R_(rs, rt, 0, 0, 0x1A)
#define MFLO(rd) R_(0, 0, rd, 0, 0x12)
#define MFHI(rd) R_(0, 0, rd, 0, 0x10)
#define MTC1(rt, fs) (0x44800000u | (uint32_t)(rt) << 16 | (uint32_t)(fs) << 11)
#define MFC1(rt, fs) (0x44000000u | (uint32_t)(rt) << 16 | (uint32_t)(fs) << 11)
#define FOP(fn, fd, fs, ft) (0x46000000u | (uint32_t)(ft) << 16 | (uint32_t)(fs) << 11 | (uint32_t)(fd) << 6 | (fn))
#define LUI_HI(a) (((a) + 0x8000u) >> 16)
#define LO(a) ((a) & 0xFFFF)

/* The test program, in the magic area: functions at offsets below 0x1000,
   "save" at 0x1000 (the start of its data), as the disc's programs */
static uint32_t *code_at(uint32_t off) {
    return (uint32_t *)(window + (GRX_MIPS_MAGIC_EE - GRX_MIPS_WIN_EE) + off);
}

static void put(uint32_t off, const uint32_t *w, int n) {
    memcpy(code_at(off), w, (size_t)n * 4);
}

static uint64_t call(uint32_t off, uint32_t a0, uint32_t a1, float *f0) {
    uint32_t args[2] = {a0, a1};
    jmp_buf jb, *outer = trap;
    uint64_t v;
    trap = &jb;
    if (setjmp(jb)) {
        trap = outer;
        grx_mips_abandon();
        failures++;
        printf("FAIL: the test program at +0x%X stopped: %s\n", off, fatal_msg);
        return 0xDEADDEADDEADDEADull;
    }
    v = grx_mips_callv_raw(GRX_MIPS_MAGIC_EE + off, args, 2, f0);
    trap = outer;
    return v;
}

/* ------------------------------------------------------------------ 1. semantics */

static void unit_tests(void) {
    uint32_t base = GRX_MIPS_MAGIC_EE;
    uint32_t units = abi_data("btlUnits")->ee, frames = abi_data("btlFrameCount")->ee;
    uint64_t v;
    float f0;
    int i;

    memset(code_at(0), 0, GRX_MIPS_AREA_SIZE);
    /* entry: return a0 + 1 */
    { uint32_t w[] = {JR(RA), ADDIU(V0, A0, 1)}; put(0, w, 2); }
    /* +0x100: delay slot runs after a jr */
    { uint32_t w[] = {ADDIU(V0, ZERO, 5), JR(RA), ADDIU(V0, V0, 1)}; put(0x100, w, 3); }
    /* +0x140: beql not taken skips its delay slot; taken runs it */
    { uint32_t w[] = {ADDIU(V0, ZERO, 1), BEQL(ZERO, V0, 2), ADDIU(V0, ZERO, 7), BEQL(ZERO, ZERO, 2),
                      ADDIU(V1, ZERO, 8), ADDIU(V1, ZERO, 9), JR(RA), NOP};
      put(0x140, w, 8); }
    /* +0x180: mult v0, a0, a1; v1 = hi */
    { uint32_t w[] = {MULT(V0, A0, A1), MFHI(V1), JR(RA), NOP}; put(0x180, w, 4); }
    /* +0x1A0: div a0 / 0: v0 = lo, v1 = hi */
    { uint32_t w[] = {DIV(A0, ZERO), MFLO(V0), MFHI(V1), JR(RA), NOP}; put(0x1A0, w, 5); }
    /* +0x1C0: unaligned 64-bit load at a0+3, store at a1+5 */
    { uint32_t w[] = {LDL(V0, 10, A0), LDR(V0, 3, A0), SDL(V0, 12, A1), SDR(V0, 5, A1), JR(RA), NOP};
      put(0x1C0, w, 6); }
    /* +0x200: FPU: f0 = a0 / a1 (as floats' bits) */
    { uint32_t w[] = {MTC1(A0, 1), MTC1(A1, 2), FOP(0x03, 0, 1, 2), JR(RA), NOP}; put(0x200, w, 5); }
    /* +0x220: f0 = sqrt(a0) */
    { uint32_t w[] = {MTC1(A0, 1), FOP(0x04, 0, 0, 1), JR(RA), NOP}; put(0x220, w, 4); }
    /* +0x240: v0 = cvt.w.s(a0) */
    { uint32_t w[] = {MTC1(A0, 1), FOP(0x24, 2, 1, 0), MFC1(V0, 2), JR(RA), NOP}; put(0x240, w, 5); }
    /* +0x260: f0 = a0 * a1 */
    { uint32_t w[] = {MTC1(A0, 1), MTC1(A1, 2), FOP(0x02, 0, 1, 2), JR(RA), NOP}; put(0x260, w, 5); }
    /* +0x280: BtlFadeStart(-1, 2, a0) through a jal with a0 set in the delay slot, then v0 = s0 */
    { uint32_t w[] = {ADDIU(SP, SP, -16), SW(RA, 0, SP), ADDIU(S0, ZERO, 77), ADDIU(A2, A0, 0),
                      ADDIU(A1, ZERO, 2), JAL(abi_func("BtlFadeStart")), ADDIU(A0, ZERO, -1), ADDU(V0, S0, ZERO),
                      LW(RA, 0, SP), JR(RA), ADDIU(SP, SP, 16)};
      put(0x280, w, 11); }
    /* +0x300: EnvLightSetPos(3, 1.5, 2.5, -1.0): a tail call (j) */
    { uint32_t w[] = {ADDIU(A0, ZERO, 3), LUI(T0, 0x3FC0), MTC1(T0, 12), LUI(T0, 0x4020), MTC1(T0, 13),
                      LUI(T0, 0xBF80), MTC1(T0, 14), (uint32_t)2 << 26 | ((abi_func("EnvLightSetPos") >> 2) & 0x03FFFFFF),
                      NOP};
      put(0x300, w, 9); }
    /* +0x340: v0 = btlFrameCount; btlFrameCount = a0 */
    { uint32_t w[] = {LUI(AT, LUI_HI(frames)), LW(V0, LO(frames), AT), LUI(AT, LUI_HI(frames)),
                      SW(A0, LO(frames), AT), JR(RA), NOP};
      put(0x340, w, 6); }
    /* +0x380: *a0 = &btlUnits[1]; v0 = *a0 (EE form again); *(a0 + 4) = the address +0x500 (code) */
    { uint32_t w[] = {LUI(T0, LUI_HI(units + 0x200)), ADDIU(T0, T0, LO(units + 0x200)), SW(T0, 0, A0),
                      LW(V0, 0, A0), LUI(T1, LUI_HI(base + 0x500)), ADDIU(T1, T1, LO(base + 0x500)),
                      SW(T1, 4, A0), JR(RA), NOP};
      put(0x380, w, 9); }
    /* +0x500: return a0 + 2 (called through its thunk) */
    { uint32_t w[] = {JR(RA), ADDIU(V0, A0, 2)}; put(0x500, w, 2); }
    /* +0x540: MgcSetNextProc(+0x500), whose fake calls it at once (a nested
       call); then return s1 + the stack's word, both set before the call */
    { uint32_t w[] = {ADDIU(SP, SP, -32), SW(RA, 0, SP), ADDIU(S1, ZERO, 1000), ADDIU(T0, ZERO, 234),
                      SW(T0, 4, SP), LUI(A0, LUI_HI(base + 0x500)), JAL(abi_func("MgcSetNextProc")),
                      ADDIU(A0, A0, LO(base + 0x500)), LW(T0, 4, SP), ADDU(V0, S1, T0), LW(RA, 0, SP), JR(RA),
                      ADDIU(SP, SP, 32)};
      put(0x540, w, 13); }
    /* +0x600: v0 = the word at +0x1010 (a pointer in the data to +0x1020) */
    { uint32_t w[] = {LUI(AT, LUI_HI(base + 0x1010)), LW(V0, LO(base + 0x1010), AT), JR(RA), NOP};
      put(0x600, w, 4); }
    /* +0x640: ld/sd keep both halves */
    { uint32_t w[] = {LD(V0, 0, A0), SD(V0, 8, A0), JR(RA), NOP}; put(0x640, w, 4); }
    /* +0x680: an ABI function's address taken with lui/addiu, stored at a0+8,
       loaded back and called through jalr */
    { uint32_t die = abi_func("BtlUnitStateMgcDie");
      uint32_t w[] = {ADDIU(SP, SP, -16), SW(RA, 0, SP), LUI(T0, LUI_HI(die)), ADDIU(T0, T0, LO(die)),
                      SW(T0, 8, A0), LW(T1, 8, A0), JALR(RA, T1), NOP, LW(RA, 0, SP), JR(RA), ADDIU(SP, SP, 16)};
      put(0x680, w, 11); }
    memcpy(code_at(0x1000), "save", 5);
    *code_at(0x1010) = base + 0x1020;
    *code_at(0x1014) = base + 0x100; /* a code address in the data: left in EE form */
    grx_mips_area_loaded(base, 0x2000);

    v = call(0, 41, 0, 0);
    CHECK(v == 42, "entry returned %lld", (long long)v);
    CHECK(grx_mips_code_end(base) == 0x6AC, "code end 0x%X", grx_mips_code_end(base));
    CHECK(*code_at(0x1010) == (uint32_t)(uintptr_t)window + (base - GRX_MIPS_WIN_EE) + 0x1020,
          "data pointer not in host form at entry: %08X", *code_at(0x1010));
    CHECK(*code_at(0x1014) == base + 0x100, "code pointer in the data changed: %08X", *code_at(0x1014));
    v = call(0x600, 0, 0, 0);
    CHECK((uint32_t)v == base + 0x1020, "data pointer loaded as %08X", (uint32_t)v);

    v = call(0x100, 0, 0, 0);
    CHECK(v == 6, "jr delay slot: %lld", (long long)v);
    v = call(0x140, 0, 0, 0);
    CHECK(v == 1 && grx_mips_cpu()->r[V1].d[0] == 8, "beql: v0 %lld v1 %lld", (long long)v,
          (long long)grx_mips_cpu()->r[V1].d[0]);
    v = call(0x180, (uint32_t)-3, 7, 0);
    CHECK((int64_t)v == -21 && (int64_t)grx_mips_cpu()->r[V1].d[0] == -1, "mult rd: %lld", (long long)v);
    v = call(0x1A0, 5, 0, 0);
    CHECK((int64_t)v == -1 && grx_mips_cpu()->r[V1].d[0] == 5, "div by zero: lo %lld hi %lld", (long long)v,
          (long long)grx_mips_cpu()->r[V1].d[0]);
    v = call(0x1A0, (uint32_t)-5, 0, 0);
    CHECK((int64_t)v == 1, "-5 / 0: lo %lld", (long long)v);
    for (i = 0; i < 32; i++) {
        scratch[i] = (uint8_t)(i + 1);
        scratch[0x100 + i] = 0xEE;
    }
    v = call(0x1C0, (uint32_t)(uintptr_t)scratch, (uint32_t)(uintptr_t)scratch + 0x100, 0);
    {
        uint64_t want;
        memcpy(&want, scratch + 3, 8);
        CHECK(v == want, "ldl/ldr: %016llX, want %016llX", (unsigned long long)v, (unsigned long long)want);
        CHECK(memcmp(scratch + 0x105, scratch + 3, 8) == 0 && scratch[0x104] == 0xEE && scratch[0x10D] == 0xEE,
              "sdl/sdr");
    }
    {
        union { float f; uint32_t u; } a, b;
        a.f = 3.0f;
        b.f = -0.0f;
        call(0x200, a.u, b.u, &f0);
        CHECK(f0 == -3.402823466e38f, "3 / -0 = %g", f0);
        a.f = 1e30f;
        b.f = 1e20f;
        call(0x260, a.u, b.u, &f0);
        CHECK(f0 == 3.402823466e38f, "overflow: %g", f0);
        a.f = 1e-30f;
        b.f = 1e-20f;
        call(0x260, a.u, b.u, &f0);
        CHECK(f0 == 0.0f, "underflow: %g", f0);
        a.f = -16.0f;
        call(0x220, a.u, 0, &f0);
        CHECK(f0 == 4.0f, "sqrt(-16) = %g", f0);
        a.f = 3e9f;
        v = call(0x240, a.u, 0, 0);
        CHECK((int32_t)v == 0x7FFFFFFF, "cvt.w.s(3e9) = %d", (int32_t)v);
        a.f = -2.75f;
        v = call(0x240, a.u, 0, 0);
        CHECK((int32_t)v == -2, "cvt.w.s(-2.75) = %d", (int32_t)v);
    }
    test_hook = 1;
    v = call(0x280, 0x1234, 0, 0);
    CHECK(rec_name && strcmp(rec_name, "BtlFadeStart") == 0 && (int32_t)rec_i[0] == -1 && rec_i[1] == 2 &&
          rec_i[2] == 0x1234 && v == 77,
          "jal with a delay slot: %s(%d, %d, %d) -> %lld", rec_name ? rec_name : "-", (int32_t)rec_i[0],
          rec_i[1], rec_i[2], (long long)v);
    rec_name = 0;
    call(0x300, 0, 0, 0);
    CHECK(rec_name && strcmp(rec_name, "EnvLightSetPos") == 0 && rec_i[0] == 3 && rec_f[0] == 1.5f &&
          rec_f[1] == 2.5f && rec_f[2] == -1.0f,
          "float arguments: %s(%d, %g, %g, %g)", rec_name ? rec_name : "-", rec_i[0], rec_f[0], rec_f[1], rec_f[2]);
    *(uint32_t *)host_of(frames) = 0x55;
    v = call(0x340, 0x66, 0, 0);
    CHECK(v == 0x55 && *(uint32_t *)host_of(frames) == 0x66, "ABI global: read %lld, now %X", (long long)v,
          *(uint32_t *)host_of(frames));
    v = call(0x380, (uint32_t)(uintptr_t)scratch, 0, 0);
    {
        uint32_t stored, thunk;
        int (*fn)(int);
        memcpy(&stored, scratch, 4);
        memcpy(&thunk, scratch + 4, 4);
        CHECK(stored == (uint32_t)(uintptr_t)host_of(units + 0x200), "stored %08X, host form is %08X", stored,
              (uint32_t)(uintptr_t)host_of(units + 0x200));
        CHECK((uint32_t)v == units + 0x200, "loaded back as %08X", (uint32_t)v);
        CHECK(grx_mips_thunk_target((void *)(uintptr_t)thunk) == base + 0x500, "code address stored as %08X",
              thunk);
        fn = (int (*)(int))(uintptr_t)thunk;
        CHECK(fn && grx_mips_thunk_target((void *)(uintptr_t)thunk) && fn(40) == 42, "thunk call");
        /* an argument in host form reaches the program in EE form */
        CHECK(fn && grx_mips_thunk_target((void *)(uintptr_t)thunk) &&
              (uint32_t)fn((int)(uintptr_t)host_of(units)) == grx_mips_to_host(units + 2),
              "thunk argument conversion");
    }
    rec_callback = (void *(*)(void))1;
    rec_i[7] = 0;
    v = call(0x540, 0, 0, 0);
    CHECK(rec_i[7] == 43 && v == 1234, "nested call: inner %u, outer %lld", rec_i[7], (long long)v);
    rec_callback = 0;
    test_hook = 0;
    {
        uint64_t q = 0x1122334455667788ull;
        memcpy(scratch, &q, 8);
        v = call(0x640, (uint32_t)(uintptr_t)scratch, 0, 0);
        CHECK(v == q && memcmp(scratch + 8, &q, 8) == 0, "ld/sd: %016llX", (unsigned long long)v);
    }
    test_hook = 1;
    rec_name = 0;
    call(0x680, (uint32_t)(uintptr_t)scratch, 0, 0);
    {
        int k;
        uint32_t stored;
        memcpy(&stored, scratch + 8, 4);
        for (k = 0; k < grx_mips_abi_nfuncs && strcmp(grx_mips_abi_funcs[k].name, "BtlUnitStateMgcDie"); k++) {
        }
        CHECK(stored == (uint32_t)(uintptr_t)func_hosts[k], "ABI function stored as %08X", stored);
        CHECK(rec_name && strcmp(rec_name, "BtlUnitStateMgcDie") == 0, "called %s", rec_name ? rec_name : "-");
    }
    test_hook = 0;
    /* soft-float doubles: dpadd(1.5, 2.25) */
    {
        GrxMipsCpu *c = grx_mips_cpu();
        double a = 1.5, b = 2.25, r;
        memcpy(&c->r[A0].d[0], &a, 8);
        memcpy(&c->r[A1].d[0], &b, 8);
        grx_mips_bi_dpadd(c, 0);
        memcpy(&r, &c->r[V0].d[0], 8);
        CHECK(r == 3.75, "dpadd: %g", r);
    }
    memset(code_at(0), 0, GRX_MIPS_AREA_SIZE);
    grx_mips_area_loaded(base, 0);
}

/* ------------------------------------------------------------------ programs */

typedef struct {
    char name[32];
    int entry;           /* DAT pack entry or GBX.PAC index */
    uint32_t base;       /* EE address it is linked at */
    uint32_t off, size;  /* in its file */
} Program;

static Program progs[600];
static int nprogs;

static uint8_t *read_file(const char *path, uint32_t *size) {
    FILE *f = fopen(path, "rb");
    long n;
    uint8_t *p;
    if (!f) {
        return 0;
    }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    p = malloc((size_t)n + 16);
    if (!p || fread(p, 1, (size_t)n, f) != (size_t)n) {
        fclose(f);
        free(p);
        return 0;
    }
    fclose(f);
    *size = (uint32_t)n;
    return p;
}

/* Which GBX slot a character program is linked for. sysGbxPac has four
   entries per unit kind of fldBtlGbxKinds (f:670480), one per slot, the
   8 party kinds first: FldBattleStart copies party entry
   (kind - 1) * 4 + i to btlPartyGbx<i>, enemy entry 4 * kind + i to
   btlEnemyGbx<i>. The jal targets into the program itself confirm it. */


static uint32_t gbx_base(const uint8_t *p, uint32_t size, int index) {
    uint32_t base = (index < 32 ? GRX_MIPS_PARTY_EE : GRX_MIPS_ENEMY_EE) + (uint32_t)(index & 3) * GRX_MIPS_AREA_SIZE;
    uint32_t a;
    for (a = 0; a + 4 <= size; a += 4) {
        uint32_t w, t;
        memcpy(&w, p + a, 4);
        if ((w >> 26) != 3) {
            continue;
        }
        t = (w & 0x03FFFFFF) << 2;
        if (t >= GRX_MIPS_PARTY_EE && t < GRX_MIPS_MAGIC_EE && t - base >= GRX_MIPS_AREA_SIZE) {

            printf("FAIL: GBX.PAC entry %d (linked at %08X?) calls %08X\n", index, base, t);
            failures++;
            break;
        }
    }
    return base;
}

static char iso_dir[512] = "iso";

static void find_programs(void) {
    char path[600];
    uint32_t size, first, i;
    uint8_t *d;
    FILE *ls;
    /* BTL_DATA/ *.DAT: entry 9 of the pack, at 0x748000 */
    snprintf(path, sizeof(path), "cmd /c dir /b \"%s\\BTL_DATA\\*.DAT\" 2>nul", iso_dir);
    ls = popen(path, "r");
    if (ls) {
        char name[64];
        while (fgets(name, sizeof(name), ls)) {
            uint32_t n, off, sz;
            name[strcspn(name, "\r\n")] = 0;
            snprintf(path, sizeof(path), "%s/BTL_DATA/%s", iso_dir, name);
            d = read_file(path, &size);
            if (!d) {
                continue;
            }
            memcpy(&n, d, 4);
            if (n > 9 && n < 4096 && 4 + 8 * n <= size) {
                memcpy(&off, d + 4 + 72, 4);
                memcpy(&sz, d + 8 + 72, 4);
                if (off && sz && off + sz <= size && nprogs < 600) {
                    Program *pr = &progs[nprogs++];
                    snprintf(pr->name, sizeof(pr->name), "BTL_DATA/%s", name);
                    pr->entry = 9;
                    pr->base = GRX_MIPS_MAGIC_EE;
                    pr->off = off;
                    pr->size = sz;
                }
            }
            free(d);
        }
        pclose(ls);
    }
    snprintf(path, sizeof(path), "%s/SYS/GBX.PAC", iso_dir);
    d = read_file(path, &size);
    if (!d) {
        return;
    }
    memcpy(&first, d, 4);
    for (i = 0; i < first / 4 && nprogs < 600; i++) {
        uint32_t o, e;
        memcpy(&o, d + 4 * i, 4);
        e = size;
        if (i + 1 < first / 4) {
            memcpy(&e, d + 4 * i + 4, 4);
        }
        if (e > o && e <= size) {
            Program *pr = &progs[nprogs++];
            snprintf(pr->name, sizeof(pr->name), "SYS/GBX.PAC");
            pr->entry = (int)i;
            pr->off = o;
            pr->size = e - o;
            pr->base = gbx_base(d + o, e - o, (int)i);
        }
    }
    free(d);
}

static int load(const Program *pr) {
    char path[600];
    uint32_t size;
    uint8_t *d;
    snprintf(path, sizeof(path), "%s/%s", iso_dir, pr->name);
    d = read_file(path, &size);
    if (!d || size > sizeof(file) || pr->size > GRX_MIPS_AREA_SIZE) {
        free(d);
        return 0;
    }
    memcpy(file, d, size);
    file_size = size;
    free(d);
    memset(grx_mips_area(pr->base), 0, GRX_MIPS_AREA_SIZE);
    memcpy(grx_mips_area(pr->base), file + pr->off, pr->size);
    grx_mips_area_loaded(pr->base, pr->size);
    return 1;
}

/* ------------------------------------------------------------------ 2. instruction kinds */

/* As the interpreter finds it (cpu.c find_code_end, tools/abi_scan.py) */
static uint32_t code_end(const uint8_t *p, uint32_t size) {
    uint32_t limit = size, a, end = 0;
    for (a = 0; a + 5 <= size; a++) {
        if (memcmp(p + a, "save", 5) == 0) {
            limit = a;
            break;
        }
    }
    for (a = 0; a + 4 <= limit; a += 4) {
        uint32_t w;
        memcpy(&w, p + a, 4);
        if (w == 0x03E00008u) {
            end = a + 8;
        }
    }
    return end;
}

static int is_control(uint32_t w) {
    uint32_t op = w >> 26;
    if (op == 0) {
        return (w & 63) == 8 || (w & 63) == 9;
    }
    if (op == 0x11 && ((w >> 21) & 31) == 8) {
        return 1;
    }
    return op == 1 || (op >= 2 && op <= 7) || (op >= 0x14 && op <= 0x17);
}

/* A key per instruction kind (opcode, function and the sub-fields that pick
   the operation) */
static uint32_t kind_of(uint32_t w) {
    uint32_t op = w >> 26;
    switch (op) {
    case 0x00: return w & 0xFC00003Fu;
    case 0x01: return w & 0xFC1F0000u;
    case 0x11: return (w & 0xFFE00000u) | (((w >> 21) & 31) >= 0x10 ? w & 63 : 0) |
                      (((w >> 21) & 31) == 8 ? w & 0x001F0000u : 0);
    case 0x12: return (w & 0xFE00003Fu) | (((w & 63) >= 0x3C) ? w & 0x7C0u : 0);
    case 0x1C: return w & 0xFC0007FFu;
    default: return w & 0xFC000000u;
    }
}

static void instruction_kinds(void) {
    static uint32_t kinds[4096], example[4096], count[4096];
    int nk = 0, i, p, bad = 0;
    for (p = 0; p < nprogs; p++) {
        uint32_t a, end;
        if (!load(&progs[p])) {
            continue;
        }
        end = code_end(file + progs[p].off, progs[p].size);
        for (a = 0; a + 4 <= end; a += 4) {
            uint32_t w, k;
            memcpy(&w, file + progs[p].off + a, 4);
            k = kind_of(w);
            for (i = 0; i < nk && kinds[i] != k; i++) {
            }
            if (i == nk && nk < 4096) {
                kinds[nk] = k;
                example[nk] = w;
                nk++;
            }
            if (i < 4096) {
                count[i]++;
            }
        }
    }
    /* Each kind in a sandbox: the word, then jr $k1; nop (no program uses
       $k1), with every other GPR pointing into a scratch buffer. The code
       may hold data (jump tables, constants): a kind that is unknown
       counts as a failure when it occurs in at least 4 places, and is
       listed otherwise. */
    for (i = 0; i < nk; i++) {
        uint32_t w = example[i];
        jmp_buf jb;
        GrxMipsCpu *c = grx_mips_cpu();
        int r;
        if (is_control(w)) {
            continue;
        }
        code_at(0x7F00)[0] = w;
        code_at(0x7F00)[1] = JR(27);
        code_at(0x7F00)[2] = NOP;
        for (r = 1; r < 32; r++) {
            c->r[r].d[0] = (uint32_t)(uintptr_t)scratch + 0x8000;
            c->r[r].d[1] = 0;
        }
        c->r[27].sd[0] = (int32_t)GRX_MIPS_RETURN;
        trap = &jb;
        if (setjmp(jb) == 0) {
            uint32_t args[8];
            for (r = 0; r < 8; r++) {
                args[r] = (uint32_t)(uintptr_t)scratch + 0x8000;
            }
            grx_mips_callv_raw(GRX_MIPS_MAGIC_EE + 0x7F00, args, 8, 0);
        } else if (grx_mips_abandon(), 0) {
        } else if (strstr(fatal_msg, "unknown instruction")) {
            char d[80];
            if (count[i] >= 4) {
                bad++;
                failures++;
                printf("FAIL: %u instructions of an unknown kind, e.g. %08X (%s)\n", count[i], w,
                       grx_mips_disasm(w, 0, d, sizeof(d)));
            } else {
                printf("note: %u words (data?) of an unknown kind, e.g. %08X (%s)\n", count[i], w,
                       grx_mips_disasm(w, 0, d, sizeof(d)));
            }
        } else if (!strstr(fatal_msg, "break")) {
            printf("note: %08X in the sandbox: %s\n", w, fatal_msg);
        }
        trap = 0;
    }
    memset(code_at(0x7F00), 0, 12);
    printf("instruction kinds: %d in the programs' code, %d unknown to the interpreter\n", nk, bad);
}

/* ------------------------------------------------------------------ 3. runs */

static FILE *logfile;
static int n_ok, n_state, n_fail, n_data;
static char reasons[64][96];
static int reason_count[64];
static int nreasons;

static void count_reason(const char *msg) {
    char key[96];
    int i;
    /* the message without its numbers */
    size_t k = 0;
    for (; *msg && k + 1 < sizeof(key); msg++) {
        if (*msg == '0' && msg[1] == 'x') {
            while (msg[1] && strchr("0123456789abcdefABCDEFx", msg[1])) {
                msg++;
            }
            key[k++] = '#';
            continue;
        }
        if (*msg >= '0' && *msg <= '9') {
            continue;
        }
        if (*msg == '(') {
            break;
        }
        key[k++] = *msg;
    }
    key[k] = 0;
    for (i = 0; i < nreasons && strcmp(reasons[i], key) != 0; i++) {
    }
    if (i == nreasons && nreasons < 64) {
        strcpy(reasons[nreasons++], key);
    }
    if (i < 64) {
        reason_count[i]++;
    }
}

/* Runs fn(arg); 0 if it returned, else the stop is in fatal_msg */
static int run1(void *fn, uint32_t a0, uint32_t a1, const char *what) {
    jmp_buf jb;
    uint32_t args[2] = {a0, a1};
    trap = &jb;
    if (setjmp(jb)) {
        trap = 0;
        grx_mips_abandon();
        logf_("  stop in %s: %s\n", what, fatal_msg);
        return 1;
    }
    logf_(" %s:\n", what);
    grx_mips_callv((uint32_t)(uintptr_t)fn, args, 2);
    trap = 0;
    return 0;
}

static void scan_hooks(const uint8_t *p, uint32_t n, const char *from) {
    uint32_t a;
    for (a = 0; a + 4 <= n; a += 4) {
        uint32_t w;
        memcpy(&w, p + a, 4);
        note_callback((void *)(uintptr_t)w, from);
    }
}

static void run_program(int pi) {
    Program *pr = &progs[pi];
    uint8_t *units = host_of(abi_data("btlUnits")->ee);
    uint8_t *defs = host_of(abi_data("btlCharDefs")->ee);
    int is_magic = pr->base == GRX_MIPS_MAGIC_EE, slot = (int)((pr->base - GRX_MIPS_PARTY_EE) >> 15);
    uint8_t *obj;
    int stopped, frame, i, ran = 0;
    if (!load(pr)) {
        return;
    }
    if (code_end(file + pr->off, pr->size) == 0) {
        n_data++; /* no code: P00x_WIN.DAT and RESULT.DAT hold images there */
        return;
    }
    if (getenv("MIPS_TEST_TRACE")) {
        fprintf(stderr, "program %d: %s entry %d at %08X\n", pi, pr->name, pr->entry, pr->base);
        quiet_logs = 0;
        grx_mips_log_calls = getenv("MIPS_TEST_CALLS") != 0;
    }
    memset(data_arena, 0, sizeof(data_arena));
    memset(null_page, 0, sizeof(null_page));
    memset(mgc_work, 0, sizeof(mgc_work));
    heap_used = 0;
    ncallbacks = 0;
    ncalls = 0;
    calllog_len = 0;
    /* the units point at their definitions; the magic's caster is unit 0 */
    for (i = 0; i < 12; i++) {
        uint32_t d = (uint32_t)(uintptr_t)(defs + 0x8C * (i & 7));
        *(uint32_t *)(units + 0x200 * i) = (uint32_t)i;
        memcpy(units + 0x200 * i + 0x1EC, &d, 4);
    }
    {
        uint32_t u = (uint32_t)(uintptr_t)units;
        memcpy(mgc_work + 0x18, &u, 4);
    }
    obj = is_magic ? mgc_work : defs + 0x8C * slot;
    logf_("%s entry %d at %08X:\n", pr->name, pr->entry, pr->base);
    stopped = run1((void *)(uintptr_t)pr->base, (uint32_t)(uintptr_t)obj, 0, "entry");
    scan_hooks(obj, is_magic ? 0xCA0 : 0x8C, "struct");
    /* frames: every hook it has set or handed over, with the struct (magic)
       or its unit (GBX) */
    for (frame = 0; !stopped && frame < 8; frame++) {
        int n = ncallbacks;
        for (i = 0; i < n && !stopped && ran < 48; i++, ran++) {
            uint32_t arg = is_magic ? (uint32_t)(uintptr_t)mgc_work : (uint32_t)(uintptr_t)(units + 0x200 * slot);
            char what[64];
            snprintf(what, sizeof(what), "hook %08X (from %s)", grx_mips_thunk_target(callbacks[i]),
                     callback_from[i]);
            stopped = run1(callbacks[i], arg, arg, what);
            scan_hooks(obj, is_magic ? 0xCA0 : 0x8C, "struct");
        }
    }
    if (!stopped) {
        n_ok++;
    } else if (strstr(fatal_msg, "unknown instruction") || strstr(fatal_msg, "doesn't have") ||
               strstr(fatal_msg, "past the code")) {
        n_fail++;
        failures++;
        printf("FAIL %s entry %d: %s\n", pr->name, pr->entry, fatal_msg);
    } else {
        n_state++;
        count_reason(fatal_msg);
    }
    if (logfile) {
        fprintf(logfile, "%s", calllog);
        fprintf(logfile, "  -> %s, %d calls, %d hooks, code ends at +0x%X\n\n", stopped ? "stopped" : "ok", ncalls,
                ncallbacks, grx_mips_code_end(pr->base));
    }
}

int main(int argc, char **argv) {
    int i;
    setvbuf(stdout, 0, _IONBF, 0);
    if (argc > 1) {
        snprintf(iso_dir, sizeof(iso_dir), "%s", argv[1]);
    }
    if (argc > 2) {
        logfile = fopen(argv[2], "w");
    }
    for (i = 0; i < grx_mips_abi_ndata; i++) {
        const GrxMipsAbiData *d = &grx_mips_abi_data[i];
        if (d->ee < EE_DATA_LO || d->ee + d->size > EE_DATA_HI) {
            printf("ABI global %s outside the test's arena\n", d->name);
            return 1;
        }
        data_hosts[i] = host_of(d->ee);
    }
    for (i = 0; i < grx_mips_abi_nfuncs && i < 1024; i++) {
        handlers[i] = grx_mips_builtin(grx_mips_abi_funcs[i].name);
        if (!handlers[i] || strncmp(grx_mips_abi_funcs[i].name, "Vu", 2) == 0) {
            handlers[i] = fake;
        }
    }
    grx_mips_setup(window, mstack, sizeof(mstack), 1);
    for (i = 0; i < grx_mips_abi_nfuncs && i < 1024; i++) {
        func_hosts[i] = &fake_fns[i];
    }
    grx_mips_set_bridge(handlers, func_hosts, data_hosts);

    unit_tests();
    printf("semantics: %d failures\n", failures);

    find_programs();
    printf("%d programs on the disc\n", nprogs);
    if (nprogs == 0) {
        printf("FAIL: no programs found in %s\n", iso_dir);
        return failures + 1;
    }
    quiet_logs = 1;
    instruction_kinds();

    grx_mips_null_page = null_page;
    grx_mips_host_check = host_ok;
    grx_mips_check_code = 1;
    grx_mips_step_limit = 5000000;
    for (i = 0; i < nprogs; i++) {
        run_program(i);
    }
    printf("runs: %d returned from every hook, %d stopped on the empty game state, %d failed; %d hold no code\n",
           n_ok, n_state, n_fail, n_data);
    for (i = 0; i < nreasons; i++) {
        printf("  %4d stopped: %s\n", reason_count[i], reasons[i]);
    }
    {
        int order[1024], n = 0, j;
        for (i = 0; i < grx_mips_abi_nfuncs && i < 1024; i++) {
            if (call_counts[i]) {
                order[n++] = i;
            }
        }
        for (i = 1; i < n; i++) {
            int k = order[i];
            for (j = i - 1; j >= 0 && call_counts[order[j]] < call_counts[k]; j--) {
                order[j + 1] = order[j];
            }
            order[j + 1] = k;
        }
        printf("bridged functions called: %d of %d; the most called:\n", n, grx_mips_abi_nfuncs);
        for (i = 0; i < n && i < 25; i++) {
            printf("  %6d %s (first call of %d programs)\n", call_counts[order[i]], grx_mips_abi_funcs[order[i]].name,
                   first_calls[order[i]]);
        }
    }
    if (logfile) {
        fclose(logfile);
    }
    printf("%s: %d failures\n", failures ? "FAILED" : "ok", failures);
    return failures;
}
