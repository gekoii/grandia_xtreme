/* The game's memory at fixed PS2 addresses (port/src/memory.c), and the
   uncached aliases (port/tools/fixups/memory.py, port/docs/hardware.md).

   The game's C uses some main-RAM areas by literal address: the heaps
   SysMain sets up (MemInit(7, 0x760000, 0x1FF0000), MemHeapCreateAt at
   0x200000 and 0x300000), the work area at 0x100000 (GRX.INI is read
   there, later the MFIFO ring), the debug memory view's addresses in the
   arena. In the port the exe image and Windows sit at those addresses,
   so each area is a host allocation below GRX_HW_LOW_LIMIT (the game
   masks addresses with
   0x0FFFFFFF and 0x07FFFFC0), made once, and the fixups replace the
   literals with the accessors below. The layout inside each block is the
   PS2's: the 0x100000-0x400000 block keeps the INI/MFIFO area, the
   battle heap and the face heap end to end, 1 MB aligned (the MFIFO ring
   is aligned to its size), and an address in the arena (0x770000,
   0x1000000) keeps its offset from the heap's start (grx_ee_ram).

   Not here: the IOP block (0x01FF0000, mapped at its own address by
   grx_hwmem_init) and the load areas of the disc's programs
   (0x700000-0x750000, the MIPS interpreter's).

   Included by the game sources the fixups change, so plain C only. */
#ifndef GRX_MEMORY_H
#define GRX_MEMORY_H

#include <stdint.h>

/* The areas, by what the game uses them for */
enum {
    GRX_AREA_INI_BUF,      /* PS2 0x00100000, 1 MB: SysIniLoad reads GRX.INI here */
    GRX_AREA_MFIFO_RING,   /* PS2 0x00100000, 1 MB, the same memory: SysMfifoStart's ring */
    GRX_AREA_BATTLE_HEAP,  /* PS2 0x00200000, 1 MB: MemHeapCreateAt, sysState.battleHeap */
    GRX_AREA_FACE_HEAP,    /* PS2 0x00300000, 1 MB: MemHeapCreateAt, sysState.faceHeap (right after the battle heap) */
    GRX_AREA_HEAP7,        /* PS2 0x00760000-0x01FF0000: MemInit's arena (7-heap table, then MemHeapCreate) */
    GRX_AREA_COUNT
};

/* Allocates every area (zero-filled). Idempotent; the accessors call it on
   first use. Call it right after grx_hwmem_init(), before SDL or anything
   else takes address space below GRX_HW_LOW_LIMIT. Stops the game
   (grx_fatal) if there is no room. */
void grx_memory_init(void);

/* Host address of an area's start and end, and its size */
uintptr_t grx_ee_area(int id);
uintptr_t grx_ee_area_end(int id);
uint32_t grx_ee_area_size(int id);
/* The PS2 address it stands for */
uint32_t grx_ee_area_ps2(int id);

/* A PS2 main-RAM address inside one of the areas, as a host address;
   any other address is returned unchanged (the port's globals, the
   heap's blocks, the IOP block, the hardware windows). For literals
   that point into an area (the debug memory view's default 0x770000) and
   for code that is handed PS2 addresses. */
uintptr_t grx_ee_ram(uint32_t ps2_addr);
/* The other way: the PS2 address a host address in one of the areas
   stands for, or 0 */
uint32_t grx_ee_ram_ps2(uintptr_t host_addr);

/* The alias bits the game ORs into main-RAM addresses (uncached,
   uncached-accelerated). On the host there is no cache to bypass, so the
   fixups turn `addr | 0x20000000` into `addr | GRX_UNCACHED`. */
#define GRX_UNCACHED     0u
#define GRX_UNCACHED_ACC 0u
/* The same alias as a word offset (0x20000000 bytes / 4), which the movie
   code ORs into an index it adds to u32 pointers (movPktFadeAlpha) */
#define GRX_UNCACHED_WORDS 0u

#endif
