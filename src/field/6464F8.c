/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/n24.h"

extern f32 miniConcertDbgTimePeak;
extern char miniConcertDbgTimeFmt[];


void MiniConcertDraw(MiniCtl *w) {
    f32 t0;
    f32 t;
    s32 st;

    t0 = GsGetFrameTimePercent();
    MiniUiResetTexCache();
    if (miniConcertWork->state == 4) {
        MiniPanelsDraw(0xBE, miniConcertWork->panels, 4);
        MiniConcertComboMarksDraw();
        MiniConcertSparksDraw();
        MiniConcertNotesDraw();
        MiniConcertBurstsDraw();
    }
    st = miniConcertWork->state;
    if (st < 5) {
        if (st >= 2) {
            MiniPanelsDraw(0xBE, miniConcertWork->menu.panels, 2);
            MiniPanelsDraw(0xBE, miniConcertWork->stage.panels, 4);
        }
    }
    t = GsGetFrameTimePercent();
    if (SysIsDebugMode() != 0) {
        t = t - t0;
        MiniDebugDrawInput();
        DbgPrintf(0x23, 5, miniConcertDbgTimeFmt, t);
        if (miniConcertDbgTimePeak < t) {
            miniConcertDbgTimePeak = t;
        }
    }
}
