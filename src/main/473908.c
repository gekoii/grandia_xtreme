/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

extern char _sceMpegChromaFormatStr[];
extern char _sceMpegProfileLevelStr[];

void _sequenceExtension(void) {
    u32 v;
    u32 a;
    u32 b;
    u32 c;
    u32 d;

    MPEG2_Flag = 1;
    _sceMpegSetIpuMp1(0);
    v = _getBits(0x1C);
    a = (v >> 1) & 0xFFF;
    b = (v >> 13) & 3;
    c = (v >> 15) & 3;
    chroma_format = (v >> 17) & 3;
    if (chroma_format != 1) {
        _sceMpegError(_sceMpegChromaFormatStr);
    }
    progressive_sequence = (v >> 19) & 1;
    profile_and_level_indication = v >> 20;
    v = _getBits(0x10);
    frame_rate_extension_d = v & 0x1F;
    frame_rate_extension_n = (v >> 5) & 3;
    low_delay = (v >> 7) & 1;
    d = v >> 8;
    if (profile_and_level_indication != 0x48 && profile_and_level_indication != 0x58) {
        _sceMpegError(_sceMpegProfileLevelStr);
    }
    horizontal_size = (c << 12) | (horizontal_size & 0xFFF);
    vertical_size = (b << 12) | (vertical_size & 0xFFF);
    bit_rate_value += a << 18;
    vbv_buffer_size += d << 10;
}
