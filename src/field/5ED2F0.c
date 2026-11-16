#include "common.h"
#include "party.h"

s32 ItemGetEquipSlot(s16 id) {
    s32 r;

    r = 0;
    switch (ptyItemTable[id].type) {
    case 5:
        r = 0;
        break;
    case 6:
        r = 1;
        break;
    case 7:
        r = 2;
        break;
    case 8:
        r = 3;
        break;
    case 9:
        r = 4;
        break;
    case 10:
        r = 5;
        break;
    }
    return r;
}
