#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "field/n14.h"

typedef struct {
    s32 w;
    s32 h;
    u8 pad8[0x5F8];
} Win;

/* fldState and sysState as {0x80-byte header, font image} x 3 (fldState.msgFontImages, sysState.fontImages):
   the original adds the 0x80 after indexing; the msgFontImages[k]/fontImages[i] forms are 153 words off. */
typedef struct {
    u8 hdr[0x80];
    Win s;
} WinSet;


#define fldWinSets ((WinSet *)&fldState)

void TxtGlyphQueueFlush(void) {
    s32 i;
    s32 j;
    s32 alt;
    WinSet *p;
    s32 k;
    WinSet *base;

    alt = EvtGlobalFlagGet(0x40F) == 1;
    for (i = 0; i < 6; i++) {
        if (txtGlyphQueue[i].clearReq != 0) {
            if (i >= 3) {
                TxtFontImageClear(&fldWinSets[i - 3].s);
            } else {
                TxtFontImageClear(&((WinSet *)&sysState)[i].s);
            }
            txtGlyphQueue[i].clearReq = 0;
        }
        if (txtGlyphQueue[i].count != 0) {
            for (j = 0; j < txtGlyphQueue[i].count; j++) {
                if (i >= 3) {
                    k = i - 3;
                    base = fldWinSets;
                    p = &base[k];
                    if (p->s.h >= txtGlyphQueue[i].row[j] * 18 &&
                        p->s.w >= txtGlyphQueue[i].col[j] * 18) {
                        if (alt) {
                            TxtFontImagePutGlyph16(&fldWinSets[i - 3].s, txtGlyphQueue[i].glyph[j], txtGlyphQueue[i].col[j], txtGlyphQueue[i].row[j]);
                        } else {
                            TxtFontImagePutGlyph18(&fldWinSets[i - 3].s, txtGlyphQueue[i].glyph[j], txtGlyphQueue[i].col[j], txtGlyphQueue[i].row[j]);
                        }
                    }
                } else {
                    p = &((WinSet *)&sysState)[i];
                    if (p->s.h >= txtGlyphQueue[i].row[j] * 18 &&
                        p->s.w >= txtGlyphQueue[i].col[j] * 18) {
                        if (alt) {
                            TxtFontImagePutGlyph16(&((WinSet *)&sysState)[i].s, txtGlyphQueue[i].glyph[j], txtGlyphQueue[i].col[j], txtGlyphQueue[i].row[j]);
                        } else {
                            TxtFontImagePutGlyph18(&((WinSet *)&sysState)[i].s, txtGlyphQueue[i].glyph[j], txtGlyphQueue[i].col[j], txtGlyphQueue[i].row[j]);
                        }
                    }
                }
            }
            txtGlyphQueue[i].count = 0;
        }
    }
}
