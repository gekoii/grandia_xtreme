#include "common.h"
#include "party.h"

void ItemGetUseData(u16 id, s32 *arg1) {
    *arg1 = (s32)ptyItemTable[id].use;
}
