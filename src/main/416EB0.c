#include "common.h"
#include "main/n01.h"

void TxtGlyphBufReset(TxtGlyphBuf *s) {
    s->cur = s->first;
    s->col = s->first % s->cols - 1;
    s->row = s->first / s->cols;
}
