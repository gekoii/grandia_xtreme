#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

void McDbgDrawCurCard(void) {
    McDbgDrawCardInfo(0x26, 0x10, McGetCurPort());
}
