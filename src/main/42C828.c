#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

void McDbgDrawBothCards(void) {
    McDbgDrawCardInfo(0x16, 0x10, mcCardInfo0);
    McDbgDrawCardInfo(0x26, 0x10, mcCardInfo1);
    McGetCurPort();
}
