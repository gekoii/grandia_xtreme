#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertFormationChange(void) {
    MiniInput *pad = &miniConcertUi->input;
    MiniConcertWork *m = miniConcertWork;
    MiniConcertStage *s = &m->stage;
    MiniSprite *cur;
    MiniSprite *o;
    MiniConcertActor *e;
    s8 idx;
    s8 v;
    u32 i;
    s8 k;

    if (pad->trigger & 0x40) {
        MiniConcertStageMenuBack();
        return;
    }
    switch (s->unk18E7) {
    case 0:
        cur = &s->sprites[27];
        if (MiniCursorMove(cur, 0, 0, 2, 2, 5, 1)) {
            MiniMsgBoxSet(s->unk18C0, 0x14);
            SndSePlay3D(0x818, 0, 0, 0);
            break;
        }
        if (pad->trigger & 0x20) {
            s->sel.b.idx = cur->cellX + cur->cellY * 3;
            s->sel.b.x = cur->cellX;
            s->sel.b.y = cur->cellY;
            s->unk18ED = s->unk18C4[s->sel.b.idx];
            if (s->unk18ED == -1) {
                SndSePlay3D(0x7D4, 0, 0, 0);
                break;
            }
            cur->active = 0;
            o = &s->sprites[26];
            o->active = 1;
            MiniSpriteSetCell(o, cur->cellX, cur->cellY);
            o = &s->sprites[25];
            o->active = 1;
            MiniSpriteSetCell(o, cur->cellX, cur->cellY);
            s->unk18E7++;
            MiniMsgBoxSet(s->unk18C0, 0x15);
            SndSePlay3D(0x819, 0, 0, 0);
        }
        break;
    case 1:
        cur = &s->sprites[25];
        if (MiniCursorMove(cur, 0, 0, 2, 2, 5, 1)) {
            MiniMsgBoxSet(s->unk18C0, 0x15);
            SndSePlay3D(0x818, 0, 0, 0);
            break;
        }
        if (pad->trigger & 0x20) {
            for (i = 0; i < 5; i++) {
                o = &s->sprites[15 + i];
                if (o->active && MINI_SPRITE_CELL(o) == MINI_SPRITE_CELL(cur)) {
                    break;
                }
            }
            k = i;
            idx = cur->cellX + cur->cellY * 3;
            v = s->unk18C4[idx];
            for (i = 0; i < 5; i++) {
                o = &s->sprites[15 + i];
                if (o->active && *(s16 *)&o->cellX == s->sel.f.xy) {
                    break;
                }
            }
            if (k < 5 && ((MiniConcertWork *)((u8 *)miniConcertWork + (i << 5)))->actors[0].unk1E == ((MiniConcertWork *)((u8 *)miniConcertWork + (k << 5)))->actors[0].unk1E) {
                SndSePlay3D(0x7D4, 0, 0, 0);
                break;
            }
            MiniSpriteSetCell(o, cur->cellX, cur->cellY);
            s->unk18C4[s->sel.b.idx] = -1;
            s->unk18C4[idx] = s->unk18ED;
            s->unk18E7--;
            e = &miniConcertWork->actors[i];
            e->unk1C = idx;
            cur->active = 0;
            s->sprites[26].active = 0;
            o = &s->sprites[27];
            o->active = 1;
            MiniSpriteSetCell(o, cur->cellX, cur->cellY);
            MiniMsgBoxSet(s->unk18C0, 0x16);
            MiniConcertCarroSePlay(e->unk1E, 0);
            if (v == -1) {
                MiniConcertFormationCheck();
            } else {
                MiniSpriteSetCell(&s->sprites[15 + k], s->sel.b.x, s->sel.b.y);
                s->unk18C4[s->sel.b.idx] = v;
                e = &miniConcertWork->actors[k];
                e->unk1C = s->sel.b.idx;
                MiniConcertCarroSePlay(e->unk1E, 0);
            }
        }
        break;
    }
}
