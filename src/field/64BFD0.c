#include "common.h"
#include "field/n24.h"

u8 MiniConcertLevelGet(void) {
    u8 i;

    for (i = 0; i < miniConcertData->hdr->unk6; i++) {
        if (miniConcertWork->unk32.h < miniConcertData->unk8[i]) {
            break;
        }
    }
    if (i == miniConcertData->hdr->unk6) {
        i--;
    }
    if (i == miniConcertData->hdr->unk6 - 1) {
        i = miniConcertData->hdr->unk6 - 2;
    }
    return i;
}
