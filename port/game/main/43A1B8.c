/* Port-only C for GphModelDrawCopyParts (0x0043A1B8), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"

void GphPartDrawCopy(GphPart *part, s32 mtx);
void GphModelDrawCopyParts(GphPart *part, s32 count);

/* GphPartDrawCopy on each of count parts (at least one) that has GA objects and isn't
   hidden, with the current model's view matrices (*(GphModel **)0x70002800,
   0x40 bytes each, one per part) */
void GphModelDrawCopyParts(GphPart *part, s32 count) {
    s32 mtx = (s32)(*(GphModel **)0x70002800)->viewMtx;

    do {
        if (part->gaObjs != NULL && !(part->flags & 8)) {
            GphPartDrawCopy(part, mtx);
        }
        part++;
        count--;
        mtx += 0x40;
    } while (count > 0);
}
