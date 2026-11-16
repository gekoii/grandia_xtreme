#include "common.h"
#include "protos/battle.h"


void BtlMenuDraw(void) {
    BtlUnitInfoDraw();
    BtlCaptionDraw();
    BtlCmdListDraw();
    MenuTacticsDraw();
    GsClearZBuffer();
    BtlCmdMenuDraw();
    MenuHelpDraw();
    MenuWidgetsDraw();
}
