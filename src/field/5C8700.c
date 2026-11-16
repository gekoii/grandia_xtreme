#include "common.h"
#include "protos/field.h"
#include "field/state.h"

/* FldMenuWinAddStyle and TxtGlyphLayoutGet take s32; these calls go through a
   u8 cast because the caller masks the id. */
void TxtMsgWinOpen(s32 id, s32 unused, s32 x, s32 y, s32 w, s32 h, s32 type) {
    switch (type) {
    case 1:
        FldMenuWinOpen(id, x + w / 2, y + h / 2, w, h, 0, 9, 0);
        FldMenuWinSetColors(id, 0x50408030, 0x50408030, 0x70204010, 0x70204010);
        break;
    default:
        FldMenuWinOpen(id, x + w / 2, y + h / 2, w, h, 0, 9, 0x180);
        FldMenuWinSetColors(id, 0x50408030, 0x50408030, 0x70204010, 0x70204010);
        break;
    case 3:
        FldMenuWinOpen(id, x + w / 2, y + h / 2 + 4, w, h, 0, 9, 0xC0);
        FldMenuWinSetColors(id, 0x80508040, 0x80284020, 0x80305050, 0x80182828);
        ((void (*)(u8, s32))FldMenuWinAddStyle)(id, 2);
        break;
    case 2:
        FldMenuWinOpen(id, x + w / 2, y + h / 2, w, h, 0, 2, 0xA00A);
        FldMenuWinSetActive(id);
        FldMenuWinSetColors(id, 0x502080A0, 0x502080A0, 0x702080A0, 0x702080A0);
        break;
    case 5:
        FldMenuWinOpen(id, x + w / 2, y + h / 2, w, h / 2 + 8, 0, 0, 0x20F0);
        ((void (*)(u8, s32))FldMenuWinAddStyle)(id, 1);
        FldMenuWinSetColors(id, 0x50804030, 0x50804030, 0x70402010, 0x70402010);
        break;
    }
    FldMenuWinSetLayer(id, 0, &txtMsgWinTexture[id].tex, (GphBatchQuad *)((s32 (*)(u8))TxtGlyphLayoutGet)(id + 3), 128.0f);
    FldMenuWinSetLayerVisible(id, 0, 0);
}
