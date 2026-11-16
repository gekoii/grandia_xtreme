#include "common.h"
#include "field/n25.h"

u16 FldFlagListIsOpen(void) {
    return fldFlagListWork->open;
}
