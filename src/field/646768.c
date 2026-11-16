#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniConcertEnd(void) {
    u8 id;
    u8 v;

    id = miniConcertData->unk4[MiniConcertLevelGet() * 5 + miniConcertWork->unk35];
    v = miniConcertData->unk10[id];
    MiniConcertRewardFlagsRoll(id);
    miniConcertWork->unk34 = MiniConcertPointsRoll();
    miniConcertWork->unk32.h = miniConcertWork->unk32.h + miniConcertWork->unk34;
    miniConcertWork->unk36 = v;
    if (miniConcertWork->unk32.h >= 10000) {
        miniConcertWork->unk32.h = 9999;
    }
    MiniConcertSaveResults();
}
