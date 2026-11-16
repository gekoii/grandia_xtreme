/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void MiniConcertComboAdd(s32 kind, f32 x, f32 y) {
    MiniConcertScore *sc;
    u8 prev;
    s32 k;
    f32 dx;
    f32 dy;
    f32 d;
    f32 ang;
    u32 i;
    MiniConcertComboMark *r;

    if (kind == 2) {
        MiniConcertComboBreak();
        return;
    }
    sc = &miniConcertWork->unk50;
    prev = sc->unk4;
    switch (prev) {
    case 0:
        sc->unk4 = kind;
        k = (u8)kind;
        break;
    case 1:
        sc->unk4 = kind;
        k = 1;
        break;
    case 2:
    default:
        sc->unk4 = kind;
        k = 2;
        break;
    }
    sc->unk0 += ((MiniConcertRules *)(k + (u32)miniConcertData->rules))->unk1A[0];
    dx = x - 560.0f + 24.0f;
    dy = y - 350.0f + 24.0f;
    d = fsqrt(dx * dx + dy * dy);
    ang = asinf(dx / d);
    if (dy < 0.0f) {
        ang = -ang;
    }
    for (i = 0; i < 128; i++) {
        if (miniConcertFx->marks[i].unk7C == 0) {
            r = &miniConcertFx->marks[i];
            r->unk7C = 1;
            r->unk7D = kind;
            r->unk74 = 0;
            r->unk76 = 0;
            r->unk68 = d;
            r->unk6C = ang;
            r->unk7A = sc->unk2;
            r->unk78 = 0;
            MiniSpriteInit(&r->spr, miniConcertData->unk40, 10, 10, 1);
            if (kind == 0) {
                r->spr.animNo = 0x10;
            } else {
                r->spr.animNo = 0x11;
            }
            sc->unk2++;
            break;
        }
    }
}
