#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniConcertCarroSoundsUpdate(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (miniConcertWork->unk8A94[i][j] != 0) {
                miniConcertWork->unk8A94[i][j]--;
                if (miniConcertWork->unk8A94[i][j] != 0) {
                    MiniConcertCarroSePlay(i, j);
                }
            }
        }
    }
}
