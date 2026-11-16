#include "common.h"
#include "field/state.h"
#include "protos/field.h"

void FldUpdateMenu(s32 arg0) {
    FldMenuUpdate();
    FldMenuWinUpdate();
    FldSpriteUpdate();
    FldSndUpdate((u8 *)camField, 3000.0f);
}
