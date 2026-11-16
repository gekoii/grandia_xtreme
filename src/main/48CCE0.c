/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void _iDisableIntc(void);

void iDisableIntc(void) {
    _iDisableIntc();
    __asm__ volatile("sync");
}
