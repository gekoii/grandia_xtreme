/*
 * The game's memory at fixed PS2 addresses (port/include/grx_memory.h).
 *
 * Two host blocks, each standing for a range of EE main RAM with the
 * PS2's layout inside it:
 *
 *   LOW    PS2 0x00100000-0x00400000 (3 MB), 1 MB aligned:
 *            +0x000000 GRX.INI buffer at boot (SysIniLoad, m:40D860),
 *                      later the MFIFO ring (SysMfifoStart, m:42FFC8)
 *            +0x100000 the battle heap (MemHeapCreateAt, m:4000C0)
 *            +0x200000 the face heap, where the battle heap ends
 *   ARENA  PS2 0x00760000-0x01FF0000 (0x1890000 bytes), MemInit's arena:
 *            +0x000000 the heap table (8 x 0x20 bytes), then the five
 *                      MemHeapCreate heaps (0x1880000 bytes in all)
 *            +0x010000 (0x00770000) the debug memory view's default base
 *                      (m:420808), +0x8A0000 (0x01000000) its fallback
 *                      (m:42B440)
 *
 * Both come from grx_hwmem_alloc_low, so they are below GRX_HW_LOW_LIMIT
 * like everything else the game masks with 0x0FFFFFFF or 0x07FFFFC0.
 * Nothing here is OS-specific: the OS part is grx_hwmem_alloc_low.
 */
#include <stddef.h>
#include <stdint.h>
#include "grx_hwmem.h"
#include "grx_memory.h"
#include "grx_platform.h"

enum { BLOCK_LOW, BLOCK_ARENA, BLOCK_COUNT };

static const struct {
    const char *name;
    uint32_t ps2;   /* the PS2 address the block's start stands for */
    uint32_t size;
    uint32_t align; /* of the host address */
    uint32_t fixed; /* where it goes (grx_hwmem.h): a save state holds addresses into it */
} blocks[BLOCK_COUNT] = {
    {"0x00100000-0x00400000 (INI buffer / MFIFO ring, battle and face heaps)", 0x00100000u, 0x00300000u, 0x00100000u,
     GRX_FIX_LOW},
    {"0x00760000-0x01FF0000 (MemInit's heap arena)", 0x00760000u, 0x01890000u, 0x00010000u, GRX_FIX_ARENA},
};

static const struct {
    int block;
    uint32_t ps2;
    uint32_t size;
} areas[GRX_AREA_COUNT] = {
    [GRX_AREA_INI_BUF] = {BLOCK_LOW, 0x00100000u, 0x00100000u},
    [GRX_AREA_MFIFO_RING] = {BLOCK_LOW, 0x00100000u, 0x00100000u},
    [GRX_AREA_BATTLE_HEAP] = {BLOCK_LOW, 0x00200000u, 0x00100000u},
    [GRX_AREA_FACE_HEAP] = {BLOCK_LOW, 0x00300000u, 0x00100000u},
    [GRX_AREA_HEAP7] = {BLOCK_ARENA, 0x00760000u, 0x01890000u},
};

static uintptr_t host[BLOCK_COUNT]; /* aligned start of each block, 0 until allocated */

void grx_memory_init(void) {
    int b;

    /* The fixed windows first, so these blocks can't land on them (only
       matters if nothing has mapped them yet: grx_hwmem_init maps once) */
    if (!host[0] && !grx_hwmem_range(GRX_HW_EE_REGS)->mapped) {
        grx_hwmem_init();
    }
    for (b = 0; b < BLOCK_COUNT; b++) {
        void *p;
        uintptr_t at;
        size_t want;

        if (host[b]) {
            continue;
        }
        if (grx_hwmem_alloc_low_at(blocks[b].fixed, blocks[b].size) != NULL) {
            host[b] = blocks[b].fixed;
            continue;
        }
        grx_hwmem_note_unfixed(blocks[b].name);
        /* alloc_low gives 64 KB alignment; ask for enough to align up */
        want = blocks[b].size + (blocks[b].align > 0x10000u ? blocks[b].align - 0x10000u : 0);
        p = grx_hwmem_alloc_low(want);
        if (p == NULL) {
            grx_fatal("no room below 0x%08X for the PS2's RAM at %s", GRX_HW_LOW_LIMIT, blocks[b].name);
        }
        at = ((uintptr_t)p + blocks[b].align - 1) & ~(uintptr_t)(blocks[b].align - 1);
        host[b] = at;
    }
}

static int check(int id) {
    if (id < 0 || id >= GRX_AREA_COUNT) {
        grx_fatal("grx_ee_area: no area %d", id);
    }
    if (!host[areas[id].block]) {
        grx_memory_init();
    }
    return areas[id].block;
}

uintptr_t grx_ee_area(int id) {
    int b = check(id);
    return host[b] + (areas[id].ps2 - blocks[b].ps2);
}

uintptr_t grx_ee_area_end(int id) {
    return grx_ee_area(id) + areas[id].size;
}

uint32_t grx_ee_area_size(int id) {
    check(id);
    return areas[id].size;
}

uint32_t grx_ee_area_ps2(int id) {
    check(id);
    return areas[id].ps2;
}

uintptr_t grx_ee_ram(uint32_t ps2_addr) {
    int b;

    for (b = 0; b < BLOCK_COUNT; b++) {
        if (ps2_addr - blocks[b].ps2 < blocks[b].size) {
            if (!host[b]) {
                grx_memory_init();
            }
            return host[b] + (ps2_addr - blocks[b].ps2);
        }
    }
    return ps2_addr;
}

uint32_t grx_ee_ram_ps2(uintptr_t host_addr) {
    int b;

    for (b = 0; b < BLOCK_COUNT; b++) {
        if (host[b] && host_addr - host[b] < blocks[b].size) {
            return blocks[b].ps2 + (uint32_t)(host_addr - host[b]);
        }
    }
    return 0;
}
