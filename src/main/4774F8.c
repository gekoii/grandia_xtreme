/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


void _extensionAndUserData(void) {
    u32 code, id;

    _nextStartCode();
    while ((code = _showBits(32)) == 0x1B5 || code == 0x1B2) {
        if (code == 0x1B5) {
            _flushBuffer(32);
            id = _getBits(4);
            id = id > 10 ? 0 : id;
            _sceMpegExtensionTable[id]();
            _nextStartCode();
        } else {
            _flushBuffer(32);
            _nextStartCode();
        }
    }
}
