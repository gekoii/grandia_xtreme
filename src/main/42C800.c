#include "common.h"
#include "pad.h"
#include "main/n03.h"

void McDbgCmdPage(void) {
    if (mcDbgPadTrig->trigger & 0x40) {
        mcDbgPage = 0;
    }
}
