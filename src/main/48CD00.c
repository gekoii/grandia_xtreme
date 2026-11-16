/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void _iEnableDmac(void);

void iEnableDmac(void) {
    _iEnableDmac();
    __asm__ volatile("sync");
}
