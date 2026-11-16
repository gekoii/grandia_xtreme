#include "common.h"
#include "field/n24.h"

void MiniConcertSetupSelect(void) {
    miniConcertWork->camFrame = 0;
    miniConcertWork->camCount = miniConcertData->hdr->unk9;
    miniConcertWork->camFirst = 0;
}
