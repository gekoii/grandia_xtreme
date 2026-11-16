#include "common.h"
#include "protos/field.h"
#include "field/n24.h"


void MiniConcertNoteSpawn(s32 idx) {
    u32 i;
    MiniConcertNote *e;
    s32 t;

    for (i = 0; i < 16; i++) {
        e = &miniConcertFx->notes[i];
        if (e->unk74 == 0) {
            e->unk72 = idx;
            e->unk70 = 0;
            t = e->unk73 = miniConcertWork->spots[idx].unk2;
            e->unk74 = 1;
            e->unk78 = 0x53;
            e->unk7A = 1;
            e->unk75 = 0;
            e->unk76 = 0;
            e->unk77 = 0;
            if ((u8)t == 1) {
                e->unk78 = 0x54;
                MiniSpriteInit(&e->spr, miniConcertData->unk40 + 10 * 8, 1, 1, 1);
                e->spr.ofsX = e->unk60;
                e->spr.ofsY = e->unk64;
            }
            return;
        }
    }
}
