/* Test of port/src/hwmem.c: maps the PS2 hardware windows at their fixed
   addresses in a 32-bit Windows process and checks they behave as memory.

   Build and run (from the repository root; any output directory):
     i686-w64-mingw32-clang -std=gnu11 -Wall -Iport/include \
         port/tests/hwmem_test.c port/src/hwmem.c -o hwmem_test.exe
     ./hwmem_test.exe

   The exe gets a 20 MB .bss (unless built with -DNO_BALLAST) so that its
   image spans about as much of the low address space as
   build/port/grandia.exe does (19 MB, most of it DWARF). The hardware
   windows must map. The IOP block at 0x01FF0000 maps only if the image
   leaves room for it; the test says which. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "grx_hwmem.h"

#ifdef NO_BALLAST
static unsigned char image_ballast[16];
#else
static unsigned char image_ballast[20 << 20];
#endif

static int failures;

#define CHECK(cond, ...)                                  \
    do {                                                  \
        if (!(cond)) {                                    \
            failures++;                                   \
            printf("FAIL %s:%d: ", __FILE__, __LINE__);   \
            printf(__VA_ARGS__);                          \
            printf("\n");                                 \
        }                                                 \
    } while (0)

static void show_ranges(const char *when) {
    int i;
    char what[MAX_PATH + 96];

    printf("%s:\n", when);
    for (i = 0; i < GRX_HW_NRANGES; i++) {
        const GrxHwRange *r = grx_hwmem_range(i);
        printf("  %-24s 0x%08X-0x%08X  %s\n", r->name, r->base, r->base + r->size - 1,
               grx_hwmem_probe(r->base, r->size, what, sizeof(what)));
    }
}

static void test_windows(void) {
    int i;

    for (i = 0; i < GRX_HW_NRANGES; i++) {
        const GrxHwRange *r = grx_hwmem_range(i);
        CHECK(r->mapped, "%s not mapped (error %lu)", r->name, r->error);
    }
    if (failures) {
        return;
    }

    /* Zero-filled */
    CHECK(GRX_HW32(0x1000A000) == 0 && GRX_HW32(0x70003FFC) == 0, "not zeroed");

    /* The game's own access patterns: a DMA start, a 64-bit GS register,
       a VU1 data quadword, the scratchpad's first and last words */
    *(volatile uint32_t *)0x1000A010 = 0x00561230;     /* D2_MADR */
    *(volatile uint32_t *)0x1000A020 = 7;              /* D2_QWC */
    *(volatile uint32_t *)0x1000A000 = 0x101;          /* D2_CHCR: STR | dir */
    *(volatile uint32_t *)0x1000F590 = 0x10000;        /* D_ENABLEW */
    *(volatile uint64_t *)0x12001000 = 0x200;          /* GS_CSR */
    *(volatile uint64_t *)0x120000E0 = 0x123456789ULL; /* GS_BGCOLOR */
    ((volatile float *)0x1100C000)[3] = 1.5f;          /* VU1 data */
    *(volatile uint32_t *)0x70000000 = 0xCAFEF00D;
    *(volatile uint32_t *)0x70003FFC = 0x12345678;
    CHECK(GRX_HW32(0x1000A010) == 0x00561230 && GRX_HW32(0x1000A020) == 7 && GRX_HW32(0x1000A000) == 0x101,
          "DMA registers don't read back");
    CHECK(GRX_HW64(0x12001000) == 0x200 && GRX_HW64(0x120000E0) == 0x123456789ULL, "GS registers don't read back");
    CHECK(((volatile float *)0x1100C000)[3] == 1.5f, "VU memory doesn't read back");
    CHECK(GRX_HW32(0x70000000) == 0xCAFEF00D && GRX_HW32(0x70003FFC) == 0x12345678, "scratchpad doesn't read back");

    /* A 16-byte quadword copy through the scratchpad (MemMove128) */
    {
        uint32_t q[4] = {1, 2, 3, 4};
        memcpy((void *)0x70002600, q, 16);
        CHECK(memcmp((void *)0x70002600, q, 16) == 0, "quadword copy");
    }

    CHECK(grx_hwmem_mapped(0x70000000, 0x4000), "scratchpad range");
    CHECK(!grx_hwmem_mapped(0x70003FFC, 8), "past the end of the scratchpad counts as mapped");
    CHECK(!grx_hwmem_mapped(0x13000000, 4), "0x13000000 counts as mapped");
    CHECK(grx_hwmem_init() == 0, "second init");
}

static void test_conflict(void) {
    void *squat;
    int failed;
    const GrxHwRange *spr = grx_hwmem_range(GRX_HW_SPR);

    grx_hwmem_shutdown();
    squat = VirtualAlloc((void *)0x70000000, 0x1000, MEM_RESERVE, PAGE_NOACCESS);
    CHECK(squat == (void *)0x70000000, "couldn't occupy 0x70000000 for the conflict test");
    printf("conflict test (expect a 'can't map the scratchpad' line):\n");
    fflush(stdout);
    failed = grx_hwmem_init();
    CHECK(failed == 1, "init with 0x70000000 taken returned %d, not 1", failed);
    CHECK(!spr->mapped && spr->blocker == 0x70000000, "scratchpad failure not recorded");
    VirtualFree(squat, 0, MEM_RELEASE);
    CHECK(grx_hwmem_init() - iop_missing() == 0, "init after the conflict is gone");
    CHECK(spr->mapped && GRX_HW32(0x70000000) == 0, "scratchpad not remapped zeroed");
}

static void test_fixed_ram(void) {
    /* What sits where the game's other fixed RAM areas are: the load
       areas of the disc's programs and the heap of MemInit(7, 0x760000,
       0x1FF0000) */
    static const struct { uint32_t base, size; const char *name; } areas[] = {
        {0x00700000, 0x50000, "program load areas"},
        {0x00760000, 0x1890000, "game heap (MemInit)"},
    };
    size_t i;
    char what[MAX_PATH + 96];

    printf("fixed RAM areas (information only):\n");
    for (i = 0; i < sizeof(areas) / sizeof(areas[0]); i++) {
        void *p = VirtualAlloc((void *)(uintptr_t)areas[i].base, areas[i].size, MEM_RESERVE | MEM_COMMIT,
                               PAGE_READWRITE);
        printf("  %-22s 0x%08X-0x%08X  %s  -> %s\n", areas[i].name, areas[i].base, areas[i].base + areas[i].size - 1,
               grx_hwmem_probe(areas[i].base, areas[i].size, what, sizeof(what)),
               p == (void *)(uintptr_t)areas[i].base ? "mappable" : "NOT mappable");
        if (p) {
            VirtualFree(p, 0, MEM_RELEASE);
        }
    }
}

static void test_low_and_aliased(void) {
    /* The game's heap is 0x1890000 bytes; it must sit below GRX_HW_LOW_LIMIT */
    uint8_t *heap = grx_hwmem_alloc_low(0x1890000);
    uint8_t *al;

    CHECK(heap != NULL, "alloc_low");
    if (heap) {
        CHECK((uintptr_t)heap + 0x1890000 <= GRX_HW_LOW_LIMIT, "alloc_low gave 0x%08lX", (unsigned long)(uintptr_t)heap);
        heap[0] = 1;
        heap[0x1890000 - 1] = 2;
        CHECK((((uintptr_t)heap & 0x07FFFFC0) == (uintptr_t)heap), "the movie code's alignment mask changes it");
        printf("alloc_low(0x1890000) = 0x%08lX\n", (unsigned long)(uintptr_t)heap);
        grx_hwmem_free_low(heap);
    }

    al = grx_hwmem_alloc_aliased(0x1890000);
    CHECK(al != NULL, "alloc_aliased");
    if (al) {
        uintptr_t a = (uintptr_t)al;
        volatile uint32_t *cached = (volatile uint32_t *)(a + 0x100);
        volatile uint32_t *unc = (volatile uint32_t *)((a + 0x100) | 0x20000000);
        volatile uint32_t *acc = (volatile uint32_t *)((a + 0x100) | 0x30000000);
        printf("alloc_aliased(0x1890000) = 0x%08lX (views at 0x%08lX, 0x%08lX)\n", (unsigned long)a,
               (unsigned long)(a | 0x20000000), (unsigned long)(a | 0x30000000));
        *unc = 0x5A5AA5A5;
        CHECK(*cached == 0x5A5AA5A5 && *acc == 0x5A5AA5A5, "a write through the uncached view isn't seen");
        *cached = 7;
        CHECK(*unc == 7, "a write through the cached view isn't seen");
        grx_hwmem_free_aliased(al);
        {
            char what[64];
            CHECK(strcmp(grx_hwmem_probe((uint32_t)(a | 0x20000000), 0x1890000, what, sizeof(what)), "free") == 0,
                  "uncached view still there after free: %s", what);
        }
    }
}

static void test_describe(void) {
    static const struct { uint32_t a; const char *want; } cases[] = {
        {0x1000A000, "DMA channel D2 GIF +0x0"},
        {0x1000B430, "DMA channel D4 toIPU +0x30"},
        {0x1000D480, "DMA channel D9 toSPR +0x80"},
        {0x10002010, "IPU"},
        {0x1000F590, "DMAC enable"},
        {0x12001000, "GS privileged register (+0x1000)"},
        {0x1100C010, "VU1 data +0x10"},
        {0x70003F60, "scratchpad (+0x3F60)"},
        {0x21FF0000, "uncached alias of 0x01FF0000"},
        {0x30561000, "uncached-accelerated alias of 0x00561000"},
        {0x00708000, "load area"},
    };
    size_t i;
    char buf[96];

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        grx_hwmem_describe(cases[i].a, buf, sizeof(buf));
        CHECK(strstr(buf, cases[i].want) != NULL, "describe(0x%08X) = \"%s\"", cases[i].a, buf);
    }
}

/* A child process that faults on an uncached alias, for the fault report */
static int fault_child(void) {
    grx_hwmem_install_fault_report();
    return (int)*(volatile uint32_t *)0x20400010;
}

static void test_fault_report(const char *self) {
    char cmd[MAX_PATH + 32];
    char line[512];
    char all[2048] = "";
    FILE *p;
    int status;

    snprintf(cmd, sizeof(cmd), "\"\"%s\" --fault 2>&1\"", self);
    p = _popen(cmd, "r");
    CHECK(p != NULL, "popen");
    if (!p) {
        return;
    }
    while (fgets(line, sizeof(line), p)) {
        if (strlen(all) + strlen(line) < sizeof(all)) {
            strcat(all, line);
        }
    }
    status = _pclose(p);
    printf("fault child exited 0x%08X and said: %s", (unsigned)status, all);
    CHECK(strstr(all, "read from 0x20400010, uncached alias of 0x00400010") != NULL, "fault report");
    CHECK(status != 0, "the fault child didn't crash");
}

int main(int argc, char **argv) {
    int failed;

    if (argc > 1 && strcmp(argv[1], "--fault") == 0) {
        return fault_child();
    }
    image_ballast[sizeof(image_ballast) - 1] = 1;
    printf("exe image 0x%08lX, ballast 0x%08lX-0x%08lX\n", (unsigned long)(uintptr_t)GetModuleHandleA(NULL),
           (unsigned long)(uintptr_t)image_ballast, (unsigned long)(uintptr_t)(image_ballast + sizeof(image_ballast)));

    show_ranges("before");
    failed = grx_hwmem_init();
    printf("grx_hwmem_init: %d failed\n", failed);
    CHECK(failed == 0, "%d windows not mapped", failed);
    show_ranges("after");

    test_windows();
    test_conflict();
    test_fixed_ram();
    test_low_and_aliased();
    test_describe();
    test_fault_report(argv[0]);

    grx_hwmem_shutdown();
    printf(failures ? "%d FAILED\n" : "all passed\n", failures);
    return failures != 0;
}
