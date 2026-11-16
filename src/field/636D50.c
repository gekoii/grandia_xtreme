#include "common.h"
#include "protos/field.h"
#include "save.h"
#include "field/state.h"


s32 FldMenuLoadUpdate(void) {
    s32 ok;
    s32 r;
    s32 id;

    ok = 0;
    r = McLoadGetState();
    if (r == 0x800) {
        ok = 1;
        FldMenuClose();
    }
    McLoadFlowUpdate();
    if (ok == 1) {
        FldLoadGameRestore();
        SysDemoSetMode(0);
        /* goto-built: the default jump falls through to join, the two range
         * cases sit after the return and jump back (the original's block order) */
        id = evtMainFlag;
        do { /* do-while(0): keeps the range tests and the default jump in this block order */
            if ((u32)(id - 0x3CD2) < 0x1E) goto a;
            if ((u32)(id - 0x3070) < 0x64) goto b;
            MapJumpRequest(0x2010, 0x100);
        } while (0);
    join:
        GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 128.0f, 10.0f);
        return 0xFF;
    b:
        MapJumpRequest(0x5A10, 0x5801);
        GphFadeSetColor(fldFades, 0.0f, 0.0f, 0.0f, 128.0f);
        goto join;
    a:
        EvtSystemFlagSet(0x96, 0);
        MapJumpRequest(0x2024, 0xD001);
        goto join;
    }
    if (r == 0x801) {
        FldMenuClose();
        return 0xFF;
    }
    return 0xA;
}
