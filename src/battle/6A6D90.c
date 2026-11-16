#include "common.h"
#include "battle/n30.h"

void IpPanelFlash(s32 arg0) {
    ((IpPanel *)(arg0 * sizeof(IpPanel) + (u32)ipPanels))->flash = 0x20;
}
