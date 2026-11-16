#include "common.h"
#include "battle/n29.h"

s32 BtlUnitInfoIsClosed(void) {
    return btlUnitInfo.unit == NULL;
}
