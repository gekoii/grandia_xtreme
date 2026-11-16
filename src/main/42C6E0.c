#include "common.h"
#include "pad.h"
#include "main/n03.h"
#include "protos/main.h"

extern s32 mcDbgCursor;
extern char mcDbgSaveStr[];
extern char mcDbgLoadStr[];
extern char mcDbgBootStr[];
extern char mcDbgCursorStr[];

void McDbgTopMenu(void) {
    u16 trig;

    McSetPollInterval(0x3C);
    trig = mcDbgPad->trigger;
    /* goto into a do-while(0): gives the original's block order (the 0x1000 arm
     * placed before its test, reached by a backward branch) and its alignment nop */
    if (!(trig & 0x4000)) goto other;
    mcDbgCursor++;
    goto end;
    do {
other:
        if (trig & 0x1000) {
            mcDbgCursor--;
        } else if (trig & 0x20) {
            mcDbgPage = 1;
            mcAccessMode = mcDbgCursor;
            McSelectPort(mcDbgCursor);
        }
    } while (0);
end:
    if (mcDbgCursor < 0) {
        mcDbgCursor = 2;
    }
    if (mcDbgCursor >= 3) {
        mcDbgCursor = 0;
    }
    DbgPrintf(2, 5, mcDbgSaveStr);
    DbgPrintf(2, 6, mcDbgLoadStr);
    DbgPrintf(2, 7, mcDbgBootStr);
    DbgPrintf(1, mcDbgCursor + 5, mcDbgCursorStr);
    McGetCurPort();
    McDbgDrawCurCard();
}
