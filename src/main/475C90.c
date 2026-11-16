/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

extern u8 _sceMpegMbAddrIncFmt[];

s32 _getMacroblockAddressIncrement(void) {
    s32 val = 0;
    u32 code;
    s32 cont;

    do {
        code = _sceMpegIpuVdec(0);
        switch (code) {
        case 0x22:
            cont = 1;
            break;
        case 0x23:
            cont = 1;
            val += 33;
            break;
        case 0:
            {
                u32 v = _showBits(11);
                if (MPEG2_Flag != 0 && v == 15) {
                    _flushBuffer(11);
                    cont = 1;
                    break;
                }
            }
            _sceMpegErrorf(_sceMpegMbAddrIncFmt, code);
            Fault_Flag = 1;
            return 1;
        default:
            val += code;
            cont = 0;
            break;
        }
    } while (cont);
    return val;
}
