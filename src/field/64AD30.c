#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertCarroChange(void) {
    MiniInput *pad = &miniConcertUi->input;
    MiniConcertWork *m = miniConcertWork;
    MiniConcertStage *s = &m->stage;
    MiniSprite *cur;
    MiniSprite *o;
    MiniConcertActor *e;
    s8 old;
    u32 i;

    if (pad->trigger & 0x40) {
        MiniConcertStageMenuBack();
        return;
    }
    switch (s->unk18E7) {
    case 0:
        cur = &s->sprites[55];
        old = cur->cellY;
        if (MiniCursorMove(cur, 0, 0, 0, 4, 5, 0)) {
            for (i = 0; s->unk18F4[cur->cellY] <= s->unk18EE[cur->cellY]; ) {
                MiniCursorMove(cur, 0, 0, 0, 4, 1, 0);
                i++;
                if (i >= 5) {
                    break;
                }
            }
            if (i == 5) {
                cur->cellY = old;
            }
            if (cur->cellY != old) {
                MiniConcertCarroSePlay(cur->cellY, 2);
                MiniSpriteMoveTo(&s->sprites[29], 0, cur->cellY * cur->cellH, 1);
                MiniMsgBoxSet(s->unk18C0, 0xE);
            }
            break;
        }
        if (pad->trigger & 0x20) {
            if (s->unk18F4[cur->cellY] == s->unk18EE[cur->cellY]) {
                break;
            }
            s->sel.b.row = cur->cellY;
            for (i = 0; i < 5; i++) {
                s->unk18EC = s->grid[cur->cellY][i];
                if (s->unk18EC != -1) {
                    break;
                }
            }
            if (i == 5) {
                break;
            }
            MiniMsgBoxSet(s->unk18C0, 0xF);
            s->sprites[27].active = 1;
            s->sprites[55].fxFlags &= ~4;
            SndSePlay3D(0x819, 0, 0, 0);
            s->unk18E7++;
        }
        break;
    case 1:
        cur = &s->sprites[27];
        if (MiniCursorMove(cur, 0, 0, 2, 2, 5, 1)) {
            MiniMsgBoxSet(s->unk18C0, 0xF);
            SndSePlay3D(0x818, 0, 0, 0);
            break;
        }
        if (pad->trigger & 0x20) {
            s->sel.b.idx = cur->cellX + cur->cellY * 3;
            s->sel.b.x = cur->cellX;
            s->sel.b.y = cur->cellY;
            s->unk18ED = s->unk18C4[s->sel.b.idx];
            if (s->unk18ED != -1) {
                if (s->unk18ED / 5 == s->sel.b.row) {
                    SndSePlay3D(0x7D4, 0, 0, 0);
                    break;
                }
                s->unk18F3--;
                s->grid[s->unk18ED / 5][s->unk18ED % 5] = s->unk18ED;
                for (i = 0; i < 5; i++) {
                    o = &s->sprites[15 + i];
                    /* cellX/cellY compared as one halfword, as the original loads the pair */
                    if (o->active && *(s16 *)&o->cellX == s->sel.f.xy) {
                        o->active = 0;
                        e = &miniConcertWork->actors[i];
                        e->unk1C = -1;
                        e->unk1D = -1;
                        s->unk18EE[e->unk1E]--;
                        MiniConcertCarroSePlay(e->unk1E, 1);
                        break;
                    }
                }
            } else if (s->unk18F3 == 5) {
                MiniMsgBoxSet(s->unk18C0, 0x13);
                MiniConcertCarroSePlay(s->sel.b.row, 1);
                break;
            }
            s->unk18F3++;
            s->unk18C4[s->sel.b.idx] = s->unk18EC;
            for (i = 0; i < 5; i++) {
                o = &s->sprites[15 + i];
                if (o->active == 0) {
                    break;
                }
            }
            o->active = 1;
            o->animNo = s->sel.b.row + 11;
            e = &miniConcertWork->actors[i];
            e->unk1C = s->sel.b.idx;
            e->unk1D = s->unk18EC;
            e->unk1E = e->unk1D / 5;
            e->unk1F = e->unk1D % 5;
            s->unk18EE[e->unk1E]++;
            MiniSpriteSetCell(o, s->sel.b.x, s->sel.b.y);
            s->grid[e->unk1E][e->unk1F] = -1;
            s->sprites[27].active = 0;
            s->unk18E7--;
            s->sprites[55].fxFlags |= 4;
            MiniConcertRosterRefresh();
            MiniConcertRosterRowAnim(e->unk1E);
            MiniConcertFormationCheck();
            MiniMsgBoxSet(s->unk18C0, 0x10);
            MiniConcertCarroSePlay(e->unk1E, 0);
        }
        break;
    }
}
