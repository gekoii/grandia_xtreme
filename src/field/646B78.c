#include "common.h"
#include "field/state.h"
#include "field/n24.h"

void MiniConcertSetupPreset(void) {
    u32 i, j;
    MiniConcertActor *s;

    miniConcertWork->songNo = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            miniConcertWork->carros[i][j]->flags &= ~1;
        }
    }
    for (i = 0; i < 5; i++) {
        s = (MiniConcertActor *)((u8 *)miniConcertWork + (i * 0x20 + 0x89F4));
        s->state = 0;
        s->unk1D = -1;
    }

    miniConcertWork->stage.unk18FA = -1;
    miniConcertWork->stage.unk18FB = -1;
    s = &miniConcertWork->actors[0];
    s->state = 1;
    s->unk1C = 9;
    s->unk1D = 0;
    s->unk1E = 0;
    s->unk1F = 0;

    miniConcertWork->camFrame = 0;
    miniConcertWork->camCount = miniConcertData->hdr->unk8 - miniConcertData->hdr->unk9;
    miniConcertWork->camFirst = miniConcertData->hdr->unk9;
}
