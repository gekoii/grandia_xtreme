/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
} Hdr;


s32 _sceMpegPackHeader(s32 arg0, Hdr *arg1) {
    u32 a;
    u32 b;
    u32 c;
    u32 n;
    u32 i;

    _sceMpegBsGetBits(arg0, 0x22);
    a = _sceMpegBsGetBits(arg0, 3);
    _sceMpegBsMarkerBit(arg0);
    b = _sceMpegBsGetBits(arg0, 0xF);
    _sceMpegBsMarkerBit(arg0);
    c = _sceMpegBsGetBits(arg0, 0xF);
    _sceMpegBsMarkerBit(arg0);
    arg1->unk0 = _sceMpegBsGetBits(arg0, 9);
    _sceMpegBsGetBits(arg0, 0x1E);
    n = _sceMpegBsGetBits(arg0, 3);
    arg1->unk8 = (a >> 2) & 1;
    arg1->unk4 = (a << 30) | (b << 15) | c;
    for (i = 0; i < n; i++) {
        _sceMpegBsGetBits(arg0, 8);
    }
    if (_sceMpegBsShowBits(arg0, 0x20) == 0x1BB) {
        arg1->unkC = 1;
        _sceMpegSystemHeader(arg0, arg1);
    } else {
        arg1->unkC = 0;
    }
    return 1;
}
