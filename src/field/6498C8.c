#include "common.h"
#include "protos/field.h"
#include "field/n24.h"

void MiniConcertCarrosDance(void) {
    u32 i;
    MiniConcertActor *a;
    MiniConcertPattern *p;

    MiniConcertCarroSoundsUpdate();
    for (i = 0; i < 5; i++) {
        a = &miniConcertWork->actors[i];
        switch (a->state) {
        case 0:
            break;
        case 1:
            if (miniConcertWork->unk50.unk6 != 1) {
                break;
            }
            a->step = 0;
            a->timer = 0;
            switch (miniConcertWork->unk50.unk5) {
            case 0:
                a->pat = miniConcertUi->input.button;
                a->state = 2;
                MiniConcertCarroStep(a, a->pats[a->pat]->dir[0]);
                break;
            case 1:
                a->sub = 0;
                a->state = 3;
                MiniConcertCarroStep(a, miniConcertData->patterns[a->sub].dir[0]);
                break;
            case 2:
                a->sub = 1;
                a->state = 3;
                MiniConcertCarroStep(a, miniConcertData->patterns[a->sub].dir[0]);
                break;
            case 3:
                a->state = 3;
                a->sub = 2;
                MiniConcertCarroStep(a, miniConcertData->patterns[a->sub].dir[0]);
                break;
            }
            switch (miniConcertWork->unk50.unk5) {
            case 0:
                p = a->pats[a->pat];
                if (a->timer == p->at[a->step]) {
                    MiniConcertCarroSoundTrigger(a, p->val[a->step]);
                }
                break;
            case 1:
            case 2:
            case 3:
                if (a->timer == miniConcertData->patterns[a->sub].at[a->step]) {
                    MiniConcertCarroSoundTrigger(a, miniConcertData->patterns[a->sub].val[a->step]);
                }
                break;
            }
            break;
        case 2:
            a->timer++;
            if (a->timer == a->pats[a->pat]->len[a->step]) {
                a->timer = 0;
                a->step++;
                if (a->step == 4 || a->pats[a->pat]->len[a->step] == 0) {
                    a->state = 1;
                    a->pat = 0xFF;
                    break;
                }
                MiniConcertCarroStep(a, a->pats[a->pat]->dir[a->step]);
            }
            p = a->pats[a->pat];
            if (a->timer == p->at[a->step]) {
                MiniConcertCarroSoundTrigger(a, p->val[a->step]);
            }
            break;
        case 3:
            a->timer++;
            if (a->timer == miniConcertData->patterns[a->sub].len[a->step]) {
                a->timer = 0;
                a->step++;
                if (a->step == 4 || miniConcertData->patterns[a->sub].len[a->step] == 0) {
                    a->state = 1;
                    a->sub = 0xFF;
                    break;
                }
                MiniConcertCarroStep(a, miniConcertData->patterns[a->sub].dir[a->step]);
            }
            if (a->timer == miniConcertData->patterns[a->sub].at[a->step]) {
                MiniConcertCarroSoundTrigger(a, miniConcertData->patterns[a->sub].val[a->step]);
            }
            break;
        case 4:
            a->state = 1;
            break;
        }
    }
    miniConcertWork->unk50.unk6 = 0;
}
