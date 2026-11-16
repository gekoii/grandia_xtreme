#include "common.h"
#include "field/event.h"

s32 EvtMsgGetPrompt(void) {
    return (s32)evtMsgCursor;
}
