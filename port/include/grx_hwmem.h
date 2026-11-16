/* Memory at the PS2 addresses the game's C uses literally (port/src/hwmem.c,
   port/docs/hardware.md).

   The game reads and writes the EE's hardware registers, VU memory, the
   GS privileged registers and the scratchpad by address. grx_hwmem_init()
   maps plain read/write memory at those addresses, so those accesses land
   somewhere instead of faulting. That memory holds the register state:
   the platform layer reads what the game wrote there (DMA channel
   registers when a transfer starts, the GS display registers at vsync) and
   writes back what the hardware would (CHCR.STR cleared and QWC 0 when a
   transfer is done, the CSR field bit at vsync). Registers whose writes
   must start work are not handled here: the functions that write them are
   replaced (see the note). */
#ifndef GRX_HWMEM_H
#define GRX_HWMEM_H

#include <stddef.h>
#include <stdint.h>

/* The mapped windows. Each base is 64 KB aligned (the Windows allocation
   granularity); size is what is committed, so an access past the end of
   the hardware's range still faults. */
#define GRX_HW_EE_REGS_BASE 0x10000000u /* timers, IPU, GIF, VIF, DMAC, INTC, SIO, D_ENABLE */
#define GRX_HW_EE_REGS_SIZE 0x00010000u
#define GRX_HW_VU_MEM_BASE  0x11000000u /* VU0 micro/data, VU1 micro/data */
#define GRX_HW_VU_MEM_SIZE  0x00010000u
#define GRX_HW_GS_PRIV_BASE 0x12000000u /* PMODE..BGCOLOR, CSR at +0x1000, IMR, BUSDIR, SIGLBLID */
#define GRX_HW_GS_PRIV_SIZE 0x00002000u
#define GRX_HW_SPR_BASE     0x70000000u /* the 16 KB scratchpad */
#define GRX_HW_SPR_SIZE     0x00004000u
/* GS_IOP.IRX's EE/IOP shared block (PS2 0x01FF0000) is not mapped here:
   it is an ordinary object (grx_iop_block() in port/include/grx_sif.h),
   and port/tools/fixups/sif.py points the game's uses at it. A fixed
   mapping in the crowded low 32 MB didn't survive SDL's DLLs. */

/* Main RAM aliases the game builds by address arithmetic */
#define GRX_PS2_UNCACHED     0x20000000u
#define GRX_PS2_UNCACHED_ACC 0x30000000u

/* Port memory the game treats as integers must stay below this: it masks
   addresses with 0x0FFFFFFF (physical) before writing MADR/TADR or DMA
   tags, and the movie code aligns heap pointers with & 0x07FFFFC0
   (m:4633B0, m:463788). So the game's heap and packet buffers go here. */
#define GRX_HW_LOW_LIMIT 0x08000000u

enum {
    GRX_HW_EE_REGS,
    GRX_HW_VU_MEM,
    GRX_HW_GS_PRIV,
    GRX_HW_SPR,
    GRX_HW_NRANGES
};

typedef struct GrxHwRange {
    const char *name;
    uint32_t base;
    uint32_t size;
    int aliased;         /* also visible at | 0x20000000 and | 0x30000000 */
    int mapped;          /* 1 once mapped */
    unsigned long error; /* GetLastError() of the failed VirtualAlloc */
    uint32_t blocker;    /* allocation base of what was already there, or 0 */
} GrxHwRange;

/* Maps every window, zero-filled. Call it first thing in main(), before
   anything that loads DLLs or allocates much. Returns the number of
   windows that could not be mapped; each failure is logged to stderr
   with what is in the way. Calling it again maps only what is missing. */
int grx_hwmem_init(void);

/* The EE's code addresses (0x00400000-0x00760000): hwmem reserves their
   free parts, so no host memory can be mistaken for them (the MIPS
   interpreter's check). _reserved: anything was reserved; _clean: [lo, hi)
   holds only free space and those reservations, nothing of the host's. */
int grx_hwmem_ee_hole_reserved(void);
int grx_hwmem_ee_hole_clean(uint32_t lo, uint32_t hi);

/* Unmaps every window (tests) */
void grx_hwmem_shutdown(void);

const GrxHwRange *grx_hwmem_range(int which);

/* Is [addr, addr + size) inside a mapped window (or an alias view of one)? */
int grx_hwmem_mapped(uint32_t addr, uint32_t size);

/* What the PS2 had at a 32-bit address ("scratchpad", "uncached alias of
   0x0056A000", ...), for diagnostics. Writes at most n bytes to buf. */
const char *grx_hwmem_describe(uint32_t addr, char *buf, size_t n);

/* What occupies [base, base + size) in this process ("free", "image
   C:\...\grandia.exe at 0x00400000", ...), for diagnostics */
const char *grx_hwmem_probe(uint32_t base, uint32_t size, char *buf, size_t n);

/* Committed, zeroed memory below GRX_HW_LOW_LIMIT, for buffers the game
   gives to DMA (its heap, packet buffers). NULL if there is no room. */
void *grx_hwmem_alloc_low(size_t size);
void grx_hwmem_free_low(void *p);
/* The same at exactly addr (64 KB aligned), or NULL if anything there is
   in use. The game's memory goes at fixed addresses this way so that a
   save state (port/src/state.c), whose data holds absolute addresses into
   it, can be loaded in a later run. */
void *grx_hwmem_alloc_low_at(uint32_t addr, size_t size);

/* Where the game's memory goes (both targets), between the IOP block's
   reservation (0x01FF0000) and the exe (0x04000000) */
#define GRX_FIX_ARENA       0x02000000u /* MemInit's heap arena, 0x1890000 bytes (memory.c) */
#define GRX_FIX_LOW         0x03900000u /* PS2 0x00100000-0x00400000, 3 MB (memory.c) */
#define GRX_FIX_MIPS_WINDOW 0x03C00000u /* the disc programs' window (programs.c) */
#define GRX_FIX_MIPS_STACK  0x03C50000u /* their stack, 0x40000 bytes */
#define GRX_FIX_DIR_SCRATCH 0x03C90000u /* SndFsMount's directory reads (sif.c) */
/* An allocation that couldn't go at its fixed address says so here (what,
   for the message): save states are then off for this run */
void grx_hwmem_note_unfixed(const char *what);
/* NULL if every fixed allocation got its address, else the first that didn't */
const char *grx_hwmem_unfixed(void);

/* Memory below GRX_HW_LOW_LIMIT that is also visible at p | 0x20000000
   and p | 0x30000000 (three views of one section), so the game's alias
   arithmetic works on it. Only for memory the port allocates (the game's
   heap): the exe's own data can't be aliased this way. NULL on failure. */
void *grx_hwmem_alloc_aliased(size_t size);
void grx_hwmem_free_aliased(void *p);

/* A vectored exception handler that, on an access violation, logs which
   PS2 region the address belongs to and the faulting instruction, then
   lets the exception continue (to the debugger, or the crash). */
void grx_hwmem_install_fault_report(void);

/* Register access for the platform layer */
#define GRX_HW32(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define GRX_HW64(a) (*(volatile uint64_t *)(uintptr_t)(a))

#endif
