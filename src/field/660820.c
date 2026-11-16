/* rodata: 0x00675CB0-0x00675CC0 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void EvtScnrMain(void) {
    evtScnrWork->cmdCount = 0;
    if (!(evtScnrWork->flags & 0x80) || (EVT_SCNR_FLAGS_WORD(evtScnrWork) & 0xC0) == 0xC0) {
        EvtScnrMainRoutine();
    }
    EvtScnrDispRoutine();
    if (EvtScnrIsDebug() != 0) {
        EvtDbgLog("scnr_main end");
    }
}
