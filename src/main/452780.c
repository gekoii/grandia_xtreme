#include "common.h"

/* Looks up a 2-byte pair in `table` and writes it to the two outputs. */
void TxtGlyphGetMetrics(s32 index, u8 *out1, u8 *out0, u8 *table) {
    if (out0 != NULL && out1 != NULL && table != NULL) {
        *out0 = table[index * 2];
        *out1 = table[index * 2 + 1];
    }
}
