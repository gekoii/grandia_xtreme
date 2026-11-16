/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"


s32 _getDmvector(void) {
    return _sceMpegIpuVdec(3);
}
