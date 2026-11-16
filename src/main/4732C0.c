/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libmpeg.h"



extern char _sceMpegVerticalSizeStr[];

void _sequenceHeader(void) {
    u32 v;
    s32 t;
    u32 *p;

    _sceMpegCurMp->sys->firstFieldStructure = 0;
    v = _getBits(0x20);
    frame_rate_code = v & 0xF;
    aspect_ratio_information = (v >> 4) & 0xF;
    t = (v >> 8) & 0xFFF;
    vertical_size = t;
    horizontal_size = v >> 20;
    if (t > 0xAF0) {
        _sceMpegError(_sceMpegVerticalSizeStr);
    }
    v = _getBits(0x1E);
    constrained_parameters_flag = v & 1;
    vbv_buffer_size = (v >> 1) & 0x3FF;
    bit_rate_value = v >> 12;
    p = &load_intra_quantizer_matrix;
    if ((*p = _getBits(1)) != 0) {
        _sceMpegIpuWait();
        _sceMpegIpuCmd(0x50000000);
        _sceMpegIpuWait();
    } else {
        _sceMpegLoadDefaultQm(0x50000000, default_intra_quantizer_matrix);
    }
    p = &load_non_intra_quantizer_matrix;
    if ((*p = _getBits(1)) != 0) {
        _sceMpegIpuWait();
        _sceMpegIpuCmd(0x58000000);
        _sceMpegIpuWait();
    } else {
        _sceMpegLoadDefaultQm(0x58000000, _sceMpegDefNonIntraQuantMatrix);
    }
    _extensionAndUserData();
    _initializeSequence(_sceMpegCurMp);
}
