/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


void _pictureDisplayExtension(void) {
    s32 n;
    s32 i;

    if (progressive_sequence != 0) {
        if (repeat_first_field == 0) {
            n = 1;
        } else {
            n = top_field_first ? 3 : 2;
        }
    } else if (picture_structure != 3) {
        n = 1;
    } else {
        n = repeat_first_field ? 3 : 2;
    }
    for (i = 0; i < n; i++) {
        frame_center_horizontal_offset[i] = _getBits(0x10);
        _getBits(1);
        frame_center_vertical_offset[i] = _getBits(0x10);
        _getBits(1);
    }
}
