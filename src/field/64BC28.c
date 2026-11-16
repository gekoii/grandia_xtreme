#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertRosterRowAnim(u8 row) {
    MiniConcertStage *s = &miniConcertWork->stage;
    MiniSprite *o;
    u32 j;
    s32 n, m;

    n = 0;
    for (j = 0; j < 5; j++) {
        o = &s->sprites[row * 5 + 30 + j];
        if (s->grid[row][j] != -1) {
            if (n == 0) {
                MiniSpriteSetCell(o, 2, row);
                MiniSpriteMoveTo(o, 0, o->cellH * row, 60);
                o->rect = row + 1;
                o->animNo = row + 6;
                o->moveFlags |= 6;
            } else {
                MiniSpriteSetCell(o, n + 2, row);
                MiniSpriteMoveTo(o, (n + 1) * o->cellW, o->cellH * row, 60);
                o->ofsY = 7;
                o->rect = row + 1;
                o->moveFlags |= 2;
                o->animNo = row + 6;
                o->sx = 0.8f;
                o->sy = 0.8f;
            }
            o->active = 1;
            n++;
        } else {
            o->active = 0;
        }
    }
}
