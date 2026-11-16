#include "common.h"
#include "field/n24.h"

void MiniConcertRankCalc(void) {
    MiniConcertWork *p = miniConcertWork;
    u16 *v = miniConcertData->songs[p->songNo]->unk24;
    u32 i;

    i = 0;
    do {
        if (*v < p->unk50.unk0) {
            break;
        }
        i++;
        v++;
    } while (i < 5);
    if (i == 5) {
        i--;
    }
    p->unk35 = i;
}
