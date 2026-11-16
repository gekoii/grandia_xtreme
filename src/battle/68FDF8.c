#include "common.h"
#include "battle/n28.h"
#include "protos/battle.h"

void MenuTacticsCursorUpdate(void) {
    s32 i = menuTacticsMember;
    s32 r = menuTacticsLists[i].sel;
    MenuCursorMoveTo(i * 160.0f + 80.0f - 44.0f, r * 18.0f + 126.0f + (r ? 4.0f : 0.0f) + 10.0f);
    MenuHelpOpen(2, r, 0, 24.0f, 340.0f);
}
