#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniConcertNotesSpawn(void) {
    u32 i;

    for (i = 0; i < miniConcertWork->unk20; i++) {
        if (miniConcertWork->spots[i].id == miniConcertWork->frame) {
            MiniConcertNoteSpawn(i);
            MiniConcertLampsFlash();
            break;
        }
    }
}
