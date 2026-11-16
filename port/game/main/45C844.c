/* Port-only C for UtilSortU64 (0x0045C844), which the decompilation still has in assembly. */
#include "common.h"

void UtilSortU64(u64 *data, u32 count, void *stack);

/* Signed distance between two pointers, as the asm's 32-bit sub */
#define DIFF(a, b) ((s32)((u32)(a) - (u32)(b)))

/* Sorts count u64s ascending (unsigned compare): an iterative quicksort
   with the last element of each range as the pivot. Ranges still to do
   are pushed as (first, last) pointer pairs, 32 bits each, on stack.

   The asm saves $s0-$s7 (all 128 bits) in the scratchpad at
   0x70000240-0x700002BF and restores them; the port's register file
   (grx_gpr) is stored there the same way. After its return there is an
   unreachable small-range sort (0x0045C9C0-0x0045CBC8), not carried over. */
void UtilSortU64(u64 *data, u32 count, void *stack) {
    u32 *sp = stack;
    u64 *lo = data;
    u64 *hi;
    u64 *i;
    u64 *j;
    u64 pivot, a, b;
    s32 d;

    __builtin_memcpy((void *)0x70000240, &grx_gpr[16], 0x80);
    if ((s32)((count - 1) * 8) <= 0) {
        return;
    }
    hi = data + (count - 1);

range:
    pivot = *hi;
    i = lo;
    j = hi;
scan:
    /* first element from i on that isn't below the pivot */
    while (*i < pivot) {
        i++;
    }
    a = *i;
    /* last element from j down that isn't above the pivot */
    while (pivot < *j) {
        j--;
    }
    b = *j;
    if (DIFF(i, j) < 0) {
        i++;
        j--;
        d = DIFF(i, j);
        j[1] = a;
        i[-1] = b;
        if (d < 0) {
            goto scan;
        }
        if (d == 0) {
            a = *i;
            if (a < pivot) {
                i++;
            }
            if (pivot < a) {
                j--;
            }
        }
    }
    if (i == j) {
        i++;
        j--;
    }
    if (DIFF(lo, j) < 0) {
        if (DIFF(i, hi) >= 0) {
            hi = j;
            goto range;
        }
        sp[0] = (u32)lo;
        sp[1] = (u32)j;
        sp += 2;
        lo = i;
        goto range;
    }
    if (DIFF(i, hi) < 0) {
        lo = i;
        goto range;
    }
    if (sp == (u32 *)stack) {
        return;
    }
    lo = (u64 *)sp[-2];
    hi = (u64 *)sp[-1];
    sp -= 2;
    goto range;
}
