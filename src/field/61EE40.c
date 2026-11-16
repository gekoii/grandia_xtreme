#include "common.h"
#include "field/menu.h"

void FldMenuSetDirectOpen(void) {
    fldMenuWork->directOpen = 1;
}

u8 FldMenuIsDirectOpen(void) {
    return fldMenuWork->directOpen;
}
