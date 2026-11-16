#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertStageMenuBack(void) {
    MiniConcertWork *m = miniConcertWork;
    MiniConcertStage *s = &m->stage;

    s->unk18E7--;
    SndSePlay3D(0x81A, 0, 0, 0);
    if (s->unk18E7 < 0) {
        s->unk18E6 = 0;
        s->unk18E7 = 0;
        MiniMsgBoxSet(&m->stage.sprites[57], s->sprites[13].cellX + 10);
        s->sprites[27].active = 0;
        s->sprites[55].active = 0;
        s->sprites[29].active = 0;
        s->sprites[25].active = 0;
        s->sprites[13].fxFlags |= 2;
    } else {
        switch (s->sprites[13].cellX) {
        case 0:
            s->sprites[27].active = 0;
            s->sprites[55].fxFlags |= 4;
            break;
        case 1:
            s->sprites[27].active = 1;
            s->sprites[26].active = 0;
            s->sprites[25].active = 0;
            break;
        case 2:
            break;
        }
    }
}
