/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

typedef struct {
    s32 mode;
    s64 a;
    s64 b;
} Req;

s32 _sceMpegDoCallback(sceMpeg *, Req *);

s32 _getHeader(void) {
    Req r;

    for (;;) {
        _nextStartCode();
        switch (_getBits(32)) {
        case 0x1B3:
            _sequenceHeader();
            break;
        case 0x1B8:
            _groupOfPicturesHeader();
            break;
        case 0x100:
            _pictureHeader();
            r.mode = 5;
            r.a = -1;
            r.b = -1;
            _sceMpegDoCallback(_sceMpegCurMp, &r);
            _sceMpegPicPts = r.a;
            _sceMpegPicDts = r.b;
            return picture_coding_type;
        case 0x1B7:
            return 0;
        }
    }
}
