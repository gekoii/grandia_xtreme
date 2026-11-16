/* Port-only C for FldMemAlloc (0x00618FD0), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

extern const char fldMemErrAllocFmt[];
extern const char fldMemErrTopFmt[];

/* Bump allocator on the field heap: rounds the size and the returned
   address to 0x80 (the +0xFE is the game's own rounding, one block more
   than needed when size is a multiple of 0x80). */
u32 FldMemAlloc(u32 size) {
    u32 cur = fldMemCur;
    u32 next = cur + ((size + 0xFE) & 0xFFFFFF80);

    fldMemCur = next;
    if (next >= fldMemEnd) {
        printf(fldMemErrAllocFmt, size);
        printf(fldMemErrTopFmt, fldMemCur, fldMemEnd);
        exit(0);
    }
    return (cur + 0xFE) & 0xFFFFFF80;
}
