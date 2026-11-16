#include "common.h"
#include "field/n20.h"

u16 EvtCharSelIsActive(void) {
    return evtCharSelWork->state;
}
