/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libmpeg.h"

typedef struct {
    s32 f0;
    s32 f4;
    s32 f8;
    u8 pad[0xAC - 0xC];
    s32 fAC;
    u8 pad2[0xD4 - 0xB0];
    s32 fD4;
} St;

typedef struct {
    u8 pad[8];
    s32 f8;
    u8 pad2[0x40 - 0xC];
    St *f40;
} Obj;

extern s32 _sceMpegOutputLastFrame(Obj *);

s32 _decodeFieldPictures(Obj *arg0, s32 arg1, s32 arg2) {
    s32 flag;
    s32 ret;
    s32 t;
    s32 t2;
    St *st;

    flag = 0;
    st = arg0->f40;
    Second_Field = 0;
    if (arg2 == -1 || arg1 < arg2) {
        flag = 1;
    }
    if (st->f8 == 0) {
        arg0->f8 = 0;
        st->f8 = 1;
    }
    if (_updatePictureBuffers(0) != 0 && flag != 0) {
        _decodePictureData(_sceMpegBitstreamFramenum, st->f4);
    }
    Second_Field = 1;
    if (_getHeader() == 0) {
        _sceMpegOutputLastFrame(arg0);
        st->f0 = 1;
        return 0;
    }
    t = (st->fD4 != 1) ? 1 : 2;
    if (picture_structure != t) {
        return -1;
    }
    ret = 0;
    t2 = 0;
    if (_updatePictureBuffers(1) != 0) {
        t2 = 1;
    }
    if (t2 != 0 && flag != 0) {
        if (_decodePictureData(_sceMpegBitstreamFramenum, st->f4) != 0) {
            ret = 1;
        }
    }
    _frameReorder(_sceMpegBitstreamFramenum, st->f4);
    Second_Field = 0;
    arg0->f8 = _sceMpegBitstreamFramenum - st->fAC;
    _sceMpegBitstreamFramenum++;
    st->f4++;
    if (flag == 0) {
        _sceMpegCallNodata(arg0);
    }
    return ret;
}
