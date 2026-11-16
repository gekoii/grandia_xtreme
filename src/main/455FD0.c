#include "common.h"
#include "main/n08.h"

void GsVif0DmaWait(void) {
    /* STR (bit 8) read as a volatile word: the bitfield gives srl/andi */
    while (*(volatile u32 *)&gsDmaChanVif0->chcr & 0x100) {
    }
}
