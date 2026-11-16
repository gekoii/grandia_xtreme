#include "common.h"
#include "field/n14.h"

void TxtGlyphQueuePush(u8 id, u16 c, u8 a, u8 b) {
    TxtGlyphQueue *p = (TxtGlyphQueue *)(id * sizeof(TxtGlyphQueue) + (u32)txtGlyphQueue);

    if (p->count < 0x100) {
        p->glyph[p->count] = c;
        p->col[p->count] = a;
        txtGlyphQueue[id].row[txtGlyphQueue[id].count] = b;
        txtGlyphQueue[id].count++;
    }
}
