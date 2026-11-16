/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"



void *_sceMpegHeapAlloc(void *, u32, s32);
void _sceMpegSetPicBufSize(void *, s32, s32);
void _sceMpegSetPicBufAddr(void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *);

void _initializeSequence(sceMpeg *arg0) {
    SceMpegSys *in;
    u32 size;

    in = arg0->sys;
    if (MPEG2_Flag == 0) {
        progressive_sequence = 1;
        chroma_format = 1;
        progressive_frame = 1;
        picture_structure = 3;
        frame_pred_frame_dct = 1;
        matrix_coefficients = 5;
    }
    mb_width = (horizontal_size + 15) >> 4;
    mb_height = (MPEG2_Flag != 0 && progressive_sequence == 0) ? ((vertical_size + 31) >> 5) * 2 : (vertical_size + 15) >> 4;
    Coded_Picture_Width = mb_width * 16;
    Coded_Picture_Height = mb_height * 16;
    if (Coded_Picture_Width != arg0->width || Coded_Picture_Height != arg0->height) {
        arg0->width = Coded_Picture_Width;
        arg0->height = Coded_Picture_Height;
        Chroma_Width = Coded_Picture_Width >> 1;
        Chroma_Height = Coded_Picture_Height >> 1;
        size = (u32)(Coded_Picture_Width * 0x180 * Coded_Picture_Height) >> 8;
        _sceMpegHeapRelease((void *)(in + 1));
        in->frameBuf[0] = _sceMpegHeapAlloc((void *)(in + 1), size, 0x40);
        in->frameBuf[1] = _sceMpegHeapAlloc((void *)(in + 1), size, 0x40);
        in->frameBuf[2] = _sceMpegHeapAlloc((void *)(in + 1), size, 0x40);
        _sceMpegSetPicBufAddr(&_sceMpegPicFrame0, &_sceMpegPicFrame1, &_sceMpegPicFrame2, &_sceMpegPicTop0, &_sceMpegPicTop1, &_sceMpegPicTop2,
                      &_sceMpegPicBottom0, &_sceMpegPicBottom1, &_sceMpegPicBottom2, in->frameBuf[0], in->frameBuf[1], in->frameBuf[2]);
        _sceMpegSetPicBufSize(&_sceMpegPicFrame0, Coded_Picture_Width, Coded_Picture_Height);
        _sceMpegSetPicBufSize(&_sceMpegPicFrame1, Coded_Picture_Width, Coded_Picture_Height);
        _sceMpegSetPicBufSize(&_sceMpegPicFrame2, Coded_Picture_Width, Coded_Picture_Height);
        _sceMpegSetPicBufSize(&_sceMpegPicTop0, Coded_Picture_Width, Coded_Picture_Height / 2);
        _sceMpegSetPicBufSize(&_sceMpegPicTop1, Coded_Picture_Width, Coded_Picture_Height / 2);
        _sceMpegSetPicBufSize(&_sceMpegPicTop2, Coded_Picture_Width, Coded_Picture_Height / 2);
        _sceMpegSetPicBufSize(&_sceMpegPicBottom0, Coded_Picture_Width, Coded_Picture_Height / 2);
        _sceMpegSetPicBufSize(&_sceMpegPicBottom1, Coded_Picture_Width, Coded_Picture_Height / 2);
        _sceMpegSetPicBufSize(&_sceMpegPicBottom2, Coded_Picture_Width, Coded_Picture_Height / 2);
    }
}
