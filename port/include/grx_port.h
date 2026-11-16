/*
 * Included before every game source in the port build (clang -include).
 *
 * The game's C is compiled unchanged apart from what port/tools/build.py
 * rewrites in its generated copy (inline asm, a few constructs clang
 * rejects); everything else the port needs is declared here or provided
 * by the platform layer in port/src/.
 */
#ifndef GRX_PORT_H
#define GRX_PORT_H

#ifndef GRX_PORT
#define GRX_PORT 1
#endif

/* The EE kernel calls named like Windows', renamed on x86-64 */
#include "grx_ee_names.h"
/* The hardware's layouts, with 32-bit address fields on x86-64 */
#include "grx_ee64.h"
#include "grx_ee.h"
/* For the calls FIXUPS in port/tools/build.py put in (grx_mips_call, ...) */
#include "grx_platform.h"

/* The game's entry point, called by the port's own main() */
#define main GameMain

/* Top-level asm for the host's assembler (symbol aliases, say), which
   port/tools/asm2c.py leaves alone: it only translates __asm__/asm, the
   game's R5900 code */
#define GRX_HOST_ASM __asm__

#endif
