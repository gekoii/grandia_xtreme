/* Native thunks for MIPS code addresses (port/include/grx_mips.h).

   A program stores its own functions into the game's function pointers
   (BtlCharDef.init, MgcWork.update, the callbacks it hands to
   BtlCamSetMode or MgcSetNextProc, ...), which the port's C then calls as
   C functions. Each such address gets one of these: a C function that
   takes up to 8 int arguments and runs the address in the interpreter.
   They are plain functions, so no executable memory is needed; a caller
   passing fewer arguments is fine with cdecl (the caller pops them).

   A slot belongs to an EE address for as long as the program that was
   loaded there when it was made; when all are taken, slots of programs
   that have since been replaced are reused. */
#include <string.h>
#include "grx_mips.h"
#include "grx_platform.h"
#include "grx_state_parts.h"

#define NTHUNKS 1000
#define HASH 4096

uint32_t grx_mips_area_gen(uint32_t ee);

static int thunk_entry(int slot, int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);

#define T(n) \
    static int thunk##n(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7) { \
        return thunk_entry(n - 1000, a0, a1, a2, a3, a4, a5, a6, a7); \
    }
#define T10(p) T(p##0) T(p##1) T(p##2) T(p##3) T(p##4) T(p##5) T(p##6) T(p##7) T(p##8) T(p##9)
#define T100(p) T10(p##0) T10(p##1) T10(p##2) T10(p##3) T10(p##4) T10(p##5) T10(p##6) T10(p##7) T10(p##8) T10(p##9)
T100(10) T100(11) T100(12) T100(13) T100(14) T100(15) T100(16) T100(17) T100(18) T100(19)
#undef T
#define T(n) thunk##n,
typedef int (*ThunkFn)(int, int, int, int, int, int, int, int);
static const ThunkFn thunks[NTHUNKS] = {
    T100(10) T100(11) T100(12) T100(13) T100(14) T100(15) T100(16) T100(17) T100(18) T100(19)
};

static struct {
    uint32_t ee;  /* 0: free */
    uint32_t gen; /* its area's generation when made */
} slots[NTHUNKS];
static int16_t hash[HASH]; /* slot + 1 */
/* thunk addresses, sorted, with their slots */
static struct { uint32_t addr; int slot; } by_addr[NTHUNKS];
static uint32_t lo_addr, hi_addr;
static int stale; /* an area was reloaded since the last reclaim */

static unsigned hash_of(uint32_t ee) {
    return (ee >> 2) * 2654435761u >> 20 & (HASH - 1);
}

void grx_mips_thunks_init(void) {
    int i, j;
    for (i = 0; i < NTHUNKS; i++) {
        uint32_t a = (uint32_t)(uintptr_t)thunks[i];
        for (j = i - 1; j >= 0 && by_addr[j].addr > a; j--) {
            by_addr[j + 1] = by_addr[j];
        }
        by_addr[j + 1].addr = a;
        by_addr[j + 1].slot = i;
    }
    lo_addr = by_addr[0].addr;
    hi_addr = by_addr[NTHUNKS - 1].addr + 1;
}

uint32_t grx_mips_thunks_lo(void) { return lo_addr; }
uint32_t grx_mips_thunks_hi(void) { return hi_addr; }
void grx_mips_thunks_area_reloaded(void) { stale = 1; }

static void rehash(void) {
    int i;
    memset(hash, 0, sizeof(hash));
    for (i = 0; i < NTHUNKS; i++) {
        if (slots[i].ee) {
            unsigned h = hash_of(slots[i].ee);
            while (hash[h]) {
                h = (h + 1) & (HASH - 1);
            }
            hash[h] = (int16_t)(i + 1);
        }
    }
}

void *grx_mips_thunk(uint32_t ee) {
    unsigned h = hash_of(ee);
    int i;
    while (hash[h]) {
        int s = hash[h] - 1;
        if (slots[s].ee == ee) {
            if (slots[s].gen != grx_mips_area_gen(ee)) {
                slots[s].gen = grx_mips_area_gen(ee); /* the same address in a newer program */
            }
            return (void *)(uintptr_t)thunks[s];
        }
        h = (h + 1) & (HASH - 1);
    }
    for (i = 0; i < NTHUNKS && slots[i].ee; i++) {
    }
    if (i == NTHUNKS && stale) {
        int freed = 0;
        for (i = 0; i < NTHUNKS; i++) {
            if (slots[i].gen != grx_mips_area_gen(slots[i].ee)) {
                slots[i].ee = 0;
                freed++;
            }
        }
        stale = 0;
        rehash();
        grx_log("mips: reused %d thunks of programs no longer loaded", freed);
        for (i = 0; i < NTHUNKS && slots[i].ee; i++) {
        }
    }
    if (i == NTHUNKS) {
        grx_fatal("mips: out of thunks (%d MIPS functions handed to native code)", NTHUNKS);
    }
    slots[i].ee = ee;
    slots[i].gen = grx_mips_area_gen(ee);
    h = hash_of(ee);
    while (hash[h]) {
        h = (h + 1) & (HASH - 1);
    }
    hash[h] = (int16_t)(i + 1);
    return (void *)(uintptr_t)thunks[i];
}

uint32_t grx_mips_thunk_target(const void *p) {
    uint32_t a = (uint32_t)(uintptr_t)p;
    int lo = 0, hi = NTHUNKS - 1;
    if (a < lo_addr || a >= hi_addr) {
        return 0;
    }
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (by_addr[mid].addr == a) {
            return slots[by_addr[mid].slot].ee;
        }
        if (by_addr[mid].addr < a) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return 0;
}

static int thunk_entry(int slot, int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7) {
    uint32_t args[8];
    if (!slots[slot].ee) {
        grx_fatal("mips: a freed thunk was called (slot %d)", slot);
    }
    args[0] = (uint32_t)a0;
    args[1] = (uint32_t)a1;
    args[2] = (uint32_t)a2;
    args[3] = (uint32_t)a3;
    args[4] = (uint32_t)a4;
    args[5] = (uint32_t)a5;
    args[6] = (uint32_t)a6;
    args[7] = (uint32_t)a7;
    return (int)grx_mips_callv(slots[slot].ee, args, 8);
}

/* Save states (grx_state_parts.h, through cpu.c's part): which EE
   functions the thunks stand for (the game keeps the thunks' addresses,
   this exe's) */
void grx_mips_thunks_state_io(GrxState *s) {
    GRX_STATE_VAR(s, slots);
    if (grx_state_loading(s)) {
        rehash();
        stale = 1;
    }
}
