/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

extern s32 _sceMpegSliceStartCodeFmt;
extern s32 _sceMpegSliceA0ErrorStr;

s32 _sliceA0(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3) {
    s32 temp_s2;
    s32 temp_v0;
    s32 temp_v0_2;

    Fault_Flag = 0;
    _nextStartCode();
    temp_v0 = _showBits(0x20);
    if ((u32) (temp_v0 - 0x101) >= 0xAFU) {
        _sceMpegErrorf(&_sceMpegSliceStartCodeFmt, temp_v0);
        return 2;
    }
    _flushBuffer(0x20);
    temp_s2 = _sliceHeader();
    temp_v0_2 = _getMacroblockAddressIncrement();
    *arg2 = temp_v0_2;
    if (Fault_Flag != 0) {
        _sceMpegError(&_sceMpegSliceA0ErrorStr);
        return 1;
    }
    *arg1 = (((((temp_s2 << 7) + (temp_v0 & 0xFF)) - 1) * mb_width) + temp_v0_2) - 1;
    *arg2 = 1;
    _sceMpegDcReset = 1;
    arg3[5] = 0;
    arg3[4] = 0;
    arg3[1] = 0;
    arg3[0] = 0;
    arg3[7] = 0;
    arg3[6] = 0;
    arg3[3] = 0;
    arg3[2] = 0;
    return 0;
}
