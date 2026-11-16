#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"

s32 FldMenuWinTextAdd(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32);

s32 FldMenuGeoPointHelpUpdate(void) {
    s32 i;
    s32 w;
    s32 x, y, n, page;

    x = 8;
    y = 8;
    n = 0;
    page = 8;
    FldMenuWinTextClear(8);
    FldMenuWinTextClear(9);
    FldMenuWinTextClear(10);
    for (i = 0; i < 37; i++) {
        w = TxtMsgGetWidth(sysGrxsysPac->msg->tbl[18], i, 20);
        if (w < 25) {
            x = 8;
            y += 22;
            if (n >= 11) {
                page++;
                n = 0;
            }
        } else {
            FldMenuWinTextAdd(page, page - 8, sysGrxsysPac->msg->tbl[18], x, y, 20, 20, n++, i, 0);
            x += w;
        }
    }
    if (padPort0[0].trigger & 0x60) {
        FldMenuClose();
        return 0xFF;
    }
    return 13;
}
