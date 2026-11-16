#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n25.h"

s32 ItemCanAdd(s16);

void MiniBallGivePrize(void) {
    MiniBallGame *s = &miniBallWork->game;
    u8 id;
    MiniBallGame *t;
    u32 i;

    if (s->winColor == s->color) {
        id = s->winColor;
        for (i = 0; i < 6; i++) {
            if (s->prizes[i].reel == id) break;
        }
        /* &s->prizes[i]; indexing the array does not match */
        t = (MiniBallGame *)((u8 *)s + (i << 5));
        if (ItemCanAdd(t->prizes[0].itemId) == -1) {
            ItemStorageAdd(t->prizes[0].itemId, 1);
            miniBallWork->prizeStored = 1;
            sysSaveData.miniBallResult = 3;
        } else {
            ItemAdd(t->prizes[0].itemId);
            sysSaveData.miniBallResult = 2;
        }
    }
}
