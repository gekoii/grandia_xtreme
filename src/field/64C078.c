#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

u8 MiniConcertPointsRoll(void) {
    u8 lv = MiniConcertLevelGet();
    u8 base = miniConcertData->unk4[lv * 5 + miniConcertWork->unk35];
    u8 q = lv / 3;

    return (base + VuRandInt(q + 2)) * 2 + 1;
}
