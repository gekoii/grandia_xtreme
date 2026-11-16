#include "common.h"
#include "protos/battle.h"
#include "sys.h"

extern u8 btlMenuDemoGlyphBuf[];
extern u8 btlMenuDemoPressStartText[];
extern u8 btlMenuDemoText[];
extern u8 btlMenuDemoCreditText[];
extern u8 btlMenuStrPressStart[];
extern u8 btlMenuStrDemonstration[];
extern u8 btlMenuStrCredit[];
void MenuTacticsInit(void);

void BtlMenuInit(void) {
    MenuWidgetsInit();
    BtlUnitInfoInit();
    BtlCaptionInit();
    BtlCmdListInit();
    MenuTacticsInit();
    MenuHelpInit();
    TxtGlyphBufInit(btlMenuDemoGlyphBuf, sysFontImage2, 0x144, 0x187);
    TxtObjSetString(btlMenuDemoPressStartText, btlMenuDemoGlyphBuf, btlMenuStrPressStart);
    TxtObjSetString(btlMenuDemoText, btlMenuDemoGlyphBuf, btlMenuStrDemonstration);
    TxtObjSetString(btlMenuDemoCreditText, btlMenuDemoGlyphBuf, btlMenuStrCredit);
}
