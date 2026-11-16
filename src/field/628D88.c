#include "common.h"
#include "field/menu.h"
#include "field/n23.h"

extern s16 itemEggCapacity;
extern s16 itemBookCapacity;
extern s16 itemSkillCapacity;

void FldMenuItemGetCount(u32 *count, s32 *kind) {
    u32 n;

    switch (fldMenuItem.category) {
    case 4:
        *kind = 0x18;
        *count = itemWork->counts[4];
        break;
    case 5:
        *kind = 5;
        *count = itemWork->counts[5];
        break;
    case 1:
        *kind = itemEggCapacity;
        *count = itemWork->counts[1];
        break;
    case 2:
        *kind = itemBookCapacity;
        *count = itemWork->counts[2];
        break;
    case 3:
        *kind = itemSkillCapacity;
        *count = itemWork->counts[3];
        break;
    case 0:
        *kind = 0x1E;
        *count = itemWork->counts[0];
        break;
    default:
        *count = 0;
        *kind = 0;
        break;
    }
    if (*count != 0 && fldMenuItem.top + fldMenuItem.cursor >= *count) {
        if (fldMenuItem.top != 0) {
            n = fldMenuItem.cursor + 1;
            fldMenuItem.top = *count - n;
        } else {
            fldMenuItem.cursor = *count - 1;
        }
    }
}
