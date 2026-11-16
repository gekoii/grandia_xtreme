#include "common.h"
#include "protos/battle.h"


void BtlMenuUpdate(void) {
    BtlUnitInfoUpdate();
    BtlCaptionUpdate();
    BtlCmdListUpdate();
    MenuTacticsAnimUpdate();
    BtlCmdMenuUpdate();
    MenuHelpUpdate();
    MenuWidgetsUpdate();
}
