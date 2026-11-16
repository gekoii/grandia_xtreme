#include "common.h"
#include "field/n27.h"

void EvtMsgInputReset(void) {
    u16 *p;

    p = evtScnrInput;
    p[0] = 0;
    p[1] = 0;
}
