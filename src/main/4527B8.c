#include "common.h"
#include "sys.h"

void TxtFontImageGetGlyphMetrics(TxtFontImage *base, u8 a, u8 b, u8 *out2, u8 *out1) {
    if (out1 != NULL && out2 != NULL && base != NULL) {
        *out1 = base->cellMetrics[a][b][0];
        *out2 = base->cellMetrics[a][b][1];
    }
}
