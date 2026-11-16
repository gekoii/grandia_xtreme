#include "common.h"
#include "protos/battle.h"
#include "battle/camera.h"
#include "battle/unit.h"
#include "battle/n28.h"
#include "sys.h"

extern u8 menuAutoCamPanelRects[];
extern u8 menuAutoCamCursorRects[];

void MenuAutoCamPanelDraw(void) {
    MenuAutoCamPanel *m = &menuAutoCamPanel;
    s32 i = 0;
    GphTexBind(0xEA);
    SprBegin(32.0f, m->slide + 398.0f);
    sprState.col[0].a = m->alpha * 128.0f;
    SprDrawRectList(menuAutoCamPanelRects);
    for (; i < 4; i++) {
        if (i != m->mode) {
            sprState.offX = i << 5;
            SprDrawColorQuad(&m->quad, m->alpha);
        }
    }
    sprState.offX = m->cursorX;
    SprDrawRectList(menuAutoCamCursorRects);
    sprState.offX = 0;
    if (m->mode >= 2) {
        BtlUnitIconDraw(&btlUnits[btlCamParamUnit], 0xC4, 0x14, 0x2000);
    }
    SprFlush();
}
