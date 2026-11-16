/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


s32 _sceMpegSystemHeader(s32 arg0, void *arg1) {
    _sceMpegBsGetBits(arg0, 0x38);
    _sceMpegBsGetBits(arg0, 0x28);
    while (_sceMpegBsShowBits(arg0, 1) == 1) {
        _sceMpegBsGetBits(arg0, 0x18);
    }
    return 1;
}
