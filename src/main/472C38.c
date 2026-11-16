/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libmpeg.h"

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0xA0];
    s32 unkAC;
} Sub;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x34];
    Sub *sub;
} Obj;


s32 _decodeFramePicture(Obj *arg0, s32 arg1, s32 arg2) {
    s32 ret;
    s32 flag;
    s32 t;
    s32 r;
    Sub *sub;

    flag = 0;
    sub = arg0->sub;
    if (!(arg2 != -1 && arg1 >= arg2)) {
        if (sub->unk8 == 0) {
            arg0->unk8 = 0;
            sub->unk8 = 1;
        }
        r = _updatePictureBuffers(0); t = 0; if (r != 0) t = _decodePictureData(_sceMpegBitstreamFramenum, sub->unk4) != 0; ret = t;
    } else {
        ret = _updatePictureBuffers(0);
        _sceMpegCallNodata(arg0);
        flag = 1;
    }
    _frameReorder(_sceMpegBitstreamFramenum, sub->unk4);
    if (picture_structure != 3 && flag == 0) {
        Second_Field = Second_Field == 0;
    }
    arg0->unk8 = _sceMpegBitstreamFramenum - sub->unkAC;
    if (Second_Field == 0) {
        _sceMpegBitstreamFramenum++;
        sub->unk4++;
    }
    return ret;
}
