#include "common.h"
#include "battle/misc.h"
#include "m2c_macros.h"

void BtlCmdListClose(void) {
    btlCmdListState = 0xF0;
}
