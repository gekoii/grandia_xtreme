#include "common.h"
#include "ptcl.h"

/* The slots seen as one table: indexing ptclFldSlots directly swaps the addu operands. */
typedef struct {
    PtclFldSprite e[64];
} SlotTbl;

void FldSpriteClearAll(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        ((SlotTbl *)ptclFldSlots)->e[i].used = 0;
    }
}
