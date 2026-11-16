#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertRosterRefresh(void) {
    MiniConcertStage *s = &miniConcertWork->stage;
    MiniSprite *o;
    u32 i, j;
    s32 n, m;

    for (i = 0; i < 5; i++) {
        n = 0;
        for (j = 0; j < 5; j++) {
            o = &s->sprites[i * 5 + 30 + j];
            if (s->grid[i][j] != -1) {
                if (n == 0) {
                    MiniSpriteSetCell(o, 0, i);
                    o->animNo = i + 1;
                    o->ofsY = 0;
                } else {
                    MiniSpriteSetCell(o, n + 1, i);
                    o->animNo = i + 1;
                    o->sx = 0.8f;
                    o->sy = 0.8f;
                    o->ofsY = 7;
                }
                n++;
                o->active = 1;
            } else {
                o->active = 0;
            }
        }
    }
}
