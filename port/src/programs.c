/* Program loading: what of it the port can't link. FIELD, BATTLE and VIEWER
   are linked in and sysProgramTable points at their entry functions
   (FIXUPS in port/tools/build.py); the code the game loads from the disc's
   data files is MIPS and runs in the interpreter (port/src/mips/,
   port/include/grx_mips.h, docs/web_port.md phase 4).

   The load areas of those programs, EE 0x708000-0x74FFFF, are the
   interpreter's window, a host buffer below 0x08000000. The game's copies
   and reads into them (port/tools/fixups/mips.py) go through
   grx_mips_load()/grx_mips_load_dest() here:

     FldBattleStart (f:619C80)  the party and enemy GBX programs from
                                sysGbxPac (btlPartyGbx0-3, btlEnemyGbx0-3)
     DbgBattleSetup (m:41FE40)  the same from the debug GBX files
     MgcUpdate (b:6B1290)       entry 9 of a BTL_DATA/ *.DAT pack to 0x748000
     MgcLoadFile (b:6B19E0)     a BATTLE/ *.GBX file read to 0x748000 (debug)

   and the calls of their entries (BtlDataLoad b:6BC620, MgcUpdate) through
   grx_mips_call(). */
#include <string.h>
#include "grx_hwmem.h"
#include "grx_mips.h"
#include "grx_platform.h"
#include "grx_state_parts.h"

#define STACK_SIZE 0x40000u

/* Memory below 0x08000000 that isn't where main and BATTLE were in EE
   memory (where an address means an ABI global or an error); an
   allocation that lands there is kept, so nothing else does, and another
   one is tried */
static void *alloc_outside_ee(uint32_t size) {
    int tries;
    for (tries = 0; tries < 16; tries++) {
        void *p = grx_hwmem_alloc_low(size);
        uint32_t a = (uint32_t)(uintptr_t)p;
        if (!p) {
            break;
        }
        if (a >= GRX_MIPS_EE_HIGH || a + size <= GRX_MIPS_EE_LOW) {
            return p;
        }
    }
    grx_fatal("mips: no memory for the programs (0x%X bytes below 0x%08X, outside 0x%08X-0x%08X)", size,
              GRX_HW_LOW_LIMIT, GRX_MIPS_EE_LOW, GRX_MIPS_EE_HIGH);
}

static void init(void) {
    char what[160];
    void *window, *stack;
    int strict;
    if (grx_mips_ready()) {
        return;
    }
    /* At fixed addresses (grx_hwmem.h), for save states */
    window = grx_hwmem_alloc_low_at(GRX_FIX_MIPS_WINDOW, GRX_MIPS_WIN_SIZE);
    if (window == NULL) {
        grx_hwmem_note_unfixed("the disc programs' window");
        window = alloc_outside_ee(GRX_MIPS_WIN_SIZE);
    }
    stack = grx_hwmem_alloc_low_at(GRX_FIX_MIPS_STACK, STACK_SIZE);
    if (stack == NULL) {
        grx_hwmem_note_unfixed("the disc programs' stack");
        stack = alloc_outside_ee(STACK_SIZE);
    }
    /* An EE address of main or BATTLE that isn't an ABI global is an error
       when nothing of the host's is there (else it may be a host pointer).
       Windows maps a small view there before main (at 0x00400000 in a
       32-bit process, 0x00420000 in a 64-bit one; nothing the game is
       handed a pointer to), which hwmem leaves out of the check; it
       reserves the free parts, which are nothing either. */
    strict = grx_hwmem_ee_hole_clean(GRX_MIPS_EE_LOW, GRX_MIPS_EE_HIGH);
    grx_hwmem_probe(GRX_MIPS_EE_LOW, GRX_MIPS_EE_HIGH - GRX_MIPS_EE_LOW, what, sizeof(what));
    if (!strict) {
        grx_log("mips: host memory at the EE addresses 0x%08X-0x%08X (%s): pointers there are ambiguous",
                GRX_MIPS_EE_LOW, GRX_MIPS_EE_HIGH, what);
    }
    grx_mips_setup(window, stack, STACK_SIZE, strict);
    grx_mips_install_bridge();
}

/* Save states: the programs' memory is part of every state, so it is set
   up before one is saved or loaded (state.c) */
void grx_programs_init(void) {
    init();
}

int grx_mips_call(unsigned int addr, unsigned int a0) {
    uint32_t arg = a0;
    init();
    return (int)grx_mips_callv(addr, &arg, 1);
}

/* MemMove128(dst, src, qwc) into a load area (dst its host address) */
void *grx_mips_load(void *dst, const void *src, int qwc) {
    uint32_t ee;
    init();
    ee = grx_mips_to_ee_any((uint32_t)(uintptr_t)dst);
    if (ee - GRX_MIPS_WIN_EE >= GRX_MIPS_WIN_SIZE || qwc < 0 ||
        (ee & 0x7FFF) + (uint32_t)qwc * 16 > GRX_MIPS_AREA_SIZE) {
        grx_fatal("mips: a program of 0x%X bytes for 0x%08X doesn't fit its load area", qwc * 16, ee);
    }
    memmove(dst, src, (size_t)qwc * 16);
    grx_mips_area_loaded(ee, (uint32_t)qwc * 16);
    return dst;
}

/* The host address a file is read to for the area at ee (size unknown) */
void *grx_mips_load_dest(unsigned int ee) {
    init();
    grx_mips_area_loaded(ee, 0);
    return grx_mips_area(ee);
}

void *grx_mips_area_host(unsigned int ee) {
    init();
    return grx_mips_area(ee);
}

int grx_program_absent(int a0, int a1) {
    (void)a0;
    (void)a1;
    grx_fatal("the game called a program that isn't on the disc (TITLE, ENDING, MCCHECK or AUTOMAP.BIN)");
}
