/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"


void _sceCdPOffHandler(void) {
    if (_sceCdSifCmdCbFunc != NULL) {
        if (_sceCdInitBusy == 0) {
            _sceCdSifCmdCbFunc(_sceCdSifCmdCbArg);
        }
    }
}
