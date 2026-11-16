/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

typedef struct {
    s32 state;
    u8 pad4[0x90];
    s32 x94;
    s32 x98;
    s32 x9C;
    s32 xA0;
    s32 xA4;
    s32 xA8;
    u8 padAC[0x28];
    s32 xD4;
    s32 xD8;
} Sub;

typedef struct {
    u8 pad[0x40];
    Sub *sub;
} Obj;

s32 _sceMpegOutputLastFrame(Obj *);
extern char _sceMpegImageAlignFmt[];

s32 _sceMpegDecodeNextPicture(Obj *mp) {
    Sub *s;
    u32 code;
    s32 r;

    code = 1;
    r = 0;
    s = mp->sub;
    s->state = 0;
    if (s->xD8 & 0x3F) {
        _sceMpegErrorf(_sceMpegImageAlignFmt, s->xD8);
        return -1;
    }
    _sceMpegPictureDone = 0;
    do {
        if (r != -1) {
            do {
                code = _getHeader();
            } while (code != 0 && picture_structure != s->xD4 && MPEG2_Flag != 0);
        }
        switch (code) {
        case 0:
            _sceMpegOutputLastFrame(mp);
            s->state = 1;
            break;
        case 1:
            s->xA8 = 0;
            s->xA4 = 0;
            s->xA0 = 0;
            r = _decodePicture(mp, 0, s->x94);
            s->xA0++;
            break;
        case 2:
            r = _decodePicture(mp, s->xA4, s->x98);
            s->xA4++;
            break;
        case 3:
        case 4:
            r = _decodePicture(mp, s->xA8, s->x9C);
            s->xA8++;
            break;
        }
    } while (_sceMpegPictureDone == 0);
    return 1;
}
