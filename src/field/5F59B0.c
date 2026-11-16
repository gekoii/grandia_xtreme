#include "common.h"
#include "party.h"
#include "protos/field.h"

s32 ItemCharSlotListBuild(s16);

s32 ItemCharUnequipBook(s16 arg0, s16 arg1) {
    s16 *p;
    s16 id;
    s32 upd;
    s32 i;

    upd = 1;
    if (arg0 >= 1000) {
        upd = 0;
        arg0 -= 1000;
    }
    for (i = 0; i < 3; i++) {
        ItemCharUnequipSkill(arg0, arg1, i);
    }
    p = (s16 *)(arg0 * 0x84 + (u32)ptyCharData + 0x3E);
    p += arg1;
    id = *p;
    *p = -1;
    if (id == -1) {
        return -1;
    }
    ptyPartyData->books[id].owner = -1;
    if (upd) {
        ItemCharSlotListBuild(arg0);
    }
    return 0;
}
