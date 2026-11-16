#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

typedef struct {
    u16 id;
    u16 pad;
} Item651048;

void AmapRollItemsByRank(s32, s32, void *, Item651048 *);

void MiniBallRollPrizes(void) {
    Item651048 buf[8];
    MiniBallPrize *s;
    u32 i;

    AmapRollItemsByRank(6, miniBallData->courses[miniBallWork->game.course - 1].prizeRank, NULL, buf);
    for (i = 0; i < 6; i++) {
        s = &miniBallWork->game.prizes[i];
        s->itemId = buf[i].id;
        s->unk2 = -1;
        s->unk4 = 0;
        s->icon = ItemGetIcon(s->itemId);
        s->spinning = 0;
        s->unkC = ItemGetUnk9AndName(s->itemId, &s->name);
    }
}
