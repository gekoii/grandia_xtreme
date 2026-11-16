/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

extern u8 _sceMpegLoadChromaIntraQmStr[];
extern u8 _sceMpegLoadChromaNonIntraQmStr[];

void _quantMatrixExtension(void) {
    s32 *p;

    p = &load_intra_quantizer_matrix;
    if ((*p = _getBits(1)) != 0) {
        _sceMpegIpuWait();
        _sceMpegIpuCmd(0x50000000);
        _sceMpegIpuWait();
    }
    p = &load_non_intra_quantizer_matrix;
    if ((*p = _getBits(1)) != 0) {
        _sceMpegIpuWait();
        _sceMpegIpuCmd(0x58000000);
        _sceMpegIpuWait();
    }
    if (_getBits(1) != 0) {
        _sceMpegError(_sceMpegLoadChromaIntraQmStr);
    }
    if (_getBits(1) != 0) {
        _sceMpegError(_sceMpegLoadChromaNonIntraQmStr);
    }
}
