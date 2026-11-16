/*
 * The game's fixed memory map (EE main RAM, 32 MB).
 *
 * Every binary is linked with this file (see the Makefile). It defines the
 * addresses that are fixed by design and do NOT follow the layout of any
 * binary we build: where each binary is loaded, the load areas of the code
 * that comes from data files on the disc, the end of the main executable's
 * area, the stack, heap and IOP areas. Everything else is either a label in
 * one of the four binaries or relative to one (tools/c_syms.py), so it moves
 * with the layout. See docs/shifting.md.
 *
 * Addresses written as literals in C (e.g. MemInit(7, 0x760000, 0x1FF0000))
 * are listed in comments: they can only change in the source.
 */

/* ---- The four binaries ------------------------------------------------- */
/* Load addresses. They must equal the vram in each splat config;
   tools/ld_script.py asserts that in every link. The main executable loads
   the overlays to the address in their sysProgramTable entry (raw words in
   its .data for FIELD and BATTLE, D_00700000 for the 0x700000 programs) and
   calls that address, so each overlay's entry is the first thing in it. */
MAIN_VRAM      = 0x00400000; /* main executable .text, ELF entry _start = +8 */
FIELD_VRAM     = 0x005B0000; /* FIELD.BIN, sysProgramTable[2], entry FldEntry */
BATTLE_VRAM    = 0x00680000; /* BATTLE.BIN, sysProgramTable[3] */
SUBPROG_VRAM   = 0x00700000; /* VIEWER.BIN (and TITLE, ENDING, MCCHECK, AUTOMAP,
                                which aren't on the disc): sysProgramTable[0,1,4,5,6].
                                The main ELF has a 0x80-byte zero stub here
                                (.lay_subprog1, label D_00700000). */
/* Room between them in the original: main's .bss ends at 0x005A6B20 (0x94E0
   bytes below FIELD); FIELD ends at 0x0067F646 (0x9BA below BATTLE, which
   is loaded while FIELD stays resident); BATTLE ends at 0x006F9350 (0x6CAC
   below btlDbgStage); VIEWER ends at 0x0070DF44. */

/* BATTLE writes a crash-stage marker word just below 0x700000 (by address) */
btlDbgStage    = 0x006FFFFC;

/* ---- Code loaded from data files ---------------------------------------- */
/* Character programs (SYS/GBX.PAC), one 0x8000-byte slot each: party
   members 0-3, enemies 0-3. BATTLE loads and calls them (b:6BC620.c calls
   (EntFn)0x708000.. as literals); the debug loaders m:41FE40 and
   f:619C80 use these names. */
GBX_SLOT_SIZE  = 0x8000;
btlPartyGbx0   = 0x00708000;
btlPartyGbx1   = 0x00710000;
btlPartyGbx2   = 0x00718000;
btlPartyGbx3   = 0x00720000;
btlEnemyGbx0   = 0x00728000;
btlEnemyGbx1   = 0x00730000;
btlEnemyGbx2   = 0x00738000;
btlEnemyGbx3   = 0x00740000;
/* Magic and skill programs (BTL_DATA/ *.DAT), linked at 0x748000 and entered
   at their start (b:6B1290.c copies one here and calls it) */
MAGIC_PROG_VRAM = 0x00748000;
func_00748000  = MAGIC_PROG_VRAM;
func_748000    = MAGIC_PROG_VRAM;
/* These programs call 157 BATTLE and 68 main functions and use 27 BATTLE and
   8 main globals by absolute address (config/abi_battle.txt,
   config/abi_main.txt), so main and BATTLE can only move at run time with
   those addresses pinned (tools/pin_link.py) or the files patched
   (docs/shifting.md). */

/* ---- Startup, stack, heap ----------------------------------------------- */
/* crt0 (_start) clears from _fbss (the start of .sbss, 0x004A9480) up to
   D_750000, calls SetupThread(threadGp, D_1FF0000, D_10000, _args, _root)
   and SetupHeap(D_750000, D_10000). D_750000 is the end of the main
   executable's area as it was linked (_end), not the end of our .bss; it is
   also newlib's heap limit (the word at 0x004A0C0C). */
D_750000       = 0x00750000; /* _end */
D_1FF0000      = 0x01FF0000; /* stack address given to SetupThread */
/* The game's own heap: MemInit(7, 0x760000, 0x1FF0000) in m:4000C0.c (literals).
   The EE/IOP shared block (magic 0x5A5AA5A5, status, command buffer) is at
   0x01FF0000: m:45EEB8.c uses it uncached as 0x21FF0000 (a literal), and the
   IOP modules GS_IOP.IRX / GS_IOPT.IRX hard-code it (lui 0x01FF). */
