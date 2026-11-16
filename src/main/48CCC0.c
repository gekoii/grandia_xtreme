/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void _iEnableIntc(void);

void iEnableIntc(void) {
    _iEnableIntc();
    __asm__ volatile("sync");
}
