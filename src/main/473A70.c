/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


void _sequenceDisplayExtension(void) {
    s32 *p;
    s32 *q;

    p = &video_format;
    q = &color_description;
    *p = _getBits(3);
    if ((*q = _getBits(1)) != 0) {
        p = &color_primaries;
        *p = _getBits(8);
        q = &transfer_characteristics;
        *q = _getBits(8);
        p = &matrix_coefficients;
        *p = _getBits(8);
    }
    p = &display_horizontal_size;
    *p = _getBits(0xE);
    q = &display_vertical_size;
    _getBits(1);
    *q = _getBits(0xE);
}
