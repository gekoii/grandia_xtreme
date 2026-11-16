/* Port-only C for GphModelMorph (0x00435668), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "protos/main.h"

void GphModelMorph(GphPart *part, u16 count, GphMotCursor *cur);

/* GphPartMorph on count parts in a row (at least one: the count is
   tested after the first) */
void GphModelMorph(GphPart *part, u16 count, GphMotCursor *cur) {
    s32 n = count;

    do {
        GphPartMorph(VuMtxStackGetTop(), part, cur);
        part++;
        GphMotCursorNextPart(cur);
        n--;
    } while (n > 0);
}
