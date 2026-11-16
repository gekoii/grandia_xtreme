/* Port-only C for GphModelDrawSimpleParts (0x00439F70), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "protos/main.h"

void GphModelDrawSimpleParts(GphPart *part, s32 count);

/* GphPartDrawSimple on each of count parts (at least one) that has GA
   objects and isn't hidden, with the current model's view matrices (*(GphModel **)0x70002800,
   0x40 bytes each, one per part) */
void GphModelDrawSimpleParts(GphPart *part, s32 count) {
    s32 mtx = (s32)(*(GphModel **)0x70002800)->viewMtx;

    do {
        if (part->gaObjs != NULL && !(part->flags & 8)) {
            GphPartDrawSimple(part, mtx);
        }
        part++;
        count--;
        mtx += 0x40;
    } while (count > 0);
}
