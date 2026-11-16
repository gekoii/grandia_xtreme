#include "common.h"
#include "save.h"
#include "field/menu.h"
#include "field/n23.h"

void FldMenuPartyPanelFlash(u8 arg0) {
    if (sysSaveData.party[arg0] != 0) {
        if (itemWork->chars[arg0].statusIcons[0] != 11 && itemWork->chars[arg0].statusIcons[1] != 11) {
            fldMenuPartyPanel.flash[arg0] = 1;
            fldMenuPartyPanel.flashTimer[arg0] = 0;
        }
    }
}
