#include "common.h"
#include "field/n15.h"

void TxtMsgCursorHide(u8 arg0) {
    txtMsgCursorWork[arg0].used = 0;
}
