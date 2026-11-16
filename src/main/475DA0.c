/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


extern u8 _sceMpegSkipPictureStr[];

s32 _pictureData(s32 arg0) {
    s32 n;
    s32 r;

    n = mb_width * mb_height;
    _sceMpegMbSlots.cur = 0;
    _sceMpegMbSlots.unk284 = 0;
    if (picture_structure != 3) {
        n >>= 1;
    }
    do {
        r = _slice(arg0, n);
    } while (r == 1 || r == 3);
    _sceMpegIpuWait();
    if (_sceMpegCheckBdec() == 0) {
        r = 2;
    }
    while ((*(volatile u32 *)0x1000D400 >> 8) & 1) {
    }
    if (r == 0) {
        _sceMpegFinishMacroblock(_sceMpegMbSlots.cur == 0);
    }
    if ((u32)(r - 1) < 2) {
        _sceMpegError(_sceMpegSkipPictureStr);
    }
    return r == 0;
}
