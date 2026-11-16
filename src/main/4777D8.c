/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


void _extraBitInformation(void) {
    while (_getBits(1) != 0) {
        _flushBuffer(8);
    }
}
