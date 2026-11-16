/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

void _iDisableDmac(void);

void iDisableDmac(void) {
    _iDisableDmac();
    __asm__ volatile("sync.l");
}
