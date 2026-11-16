/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


s32 _sliceHeader(void) {
    s32 *p;

    p = &quantizer_scale_code;
    *p = _getBits(5);
    if (_getBits(1) != 0) {
        p = &intra_slice;
        *p = _getBits(1);
        _flushBuffer(7);
        _extraBitInformation();
    } else {
        intra_slice = 0;
    }
    return 0;
}
