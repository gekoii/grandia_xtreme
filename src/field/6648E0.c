/* rodata: 0x00676268-0x00676278 */
#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void EvtScnrSetDebug(s32 arg0) {
    if (arg0 == 1) {
        evtScnrWork->flags |= EVT_SCNR_DEBUG;
    } else if (arg0 == 0) {
        evtScnrWork->flags &= ~EVT_SCNR_DEBUG;
    } else {
        char buf[32] = "debug mode set";

        EvtDbgPrint(buf);
    }
}
