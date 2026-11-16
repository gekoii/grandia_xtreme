#include "common.h"
#include "field/n14.h"

/* Corner alphas of sprite j of a glyph layout, addressed from the layout
 * base (the 0x10-byte header is folded into the offsets): the original adds
 * j << 6 to the layout base; txtGlyphLayout[i].sprites[j] forms are 44+ words off. */
typedef struct {
    u8 pad0[0x23];
    u8 a0;
    u8 pad24[0xB];
    u8 a1;
    u8 pad30[0xB];
    u8 a2;
    u8 pad3C[0xB];
    u8 a3;
} Item;

#define G(i) ((TxtGlyphLayout *)((i) * 0x8010 + (u32)txtGlyphLayout))
#define P ((Item *)((u8 *)G(i) + (j << 6)))

void TxtGlyphFadeIn(void) {
    s32 i;
    s32 j;
    Item *p;
    u8 v;

    for (i = 0; i < 6; i++) {
        for (j = 0; j < G(i)->count * 2; j++) {
            if (P->a0 != 0x80) {
                P->a0 += 0x20;
                if (P->a0 > 0x80) {
                    P->a0 = 0x80;
                }
                p = P;
                v = p->a0;
                p->a1 = v;
                p->a2 = v;
                p->a3 = v;
            }
        }
    }
}
