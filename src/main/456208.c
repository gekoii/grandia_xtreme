#include "common.h"
#include "main/n08.h"

void GsToSprDmaWait(void) {
    /* STR (bit 8) read as a volatile word: the bitfield gives srl/andi */
    while (*(volatile u32 *)&gsDmaChanToSpr->chcr & 0x100) {
    }
}
