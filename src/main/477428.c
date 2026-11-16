/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


void _pictureHeader(void) {
    s32 *p;

    p = &temporal_reference;
    *p = _getBits(0xA);
    p = &picture_coding_type;
    *p = _getBits(3);
    p = &vbv_delay;
    *p = _getBits(0x10);
    if (picture_coding_type == 2 || picture_coding_type == 3) {
        p = &full_pel_forward_vector;
        *p = _getBits(1);
        p = &forward_f_code;
        *p = _getBits(3);
    }
    if (picture_coding_type == 3) {
        p = &full_pel_backward_vector;
        *p = _getBits(1);
        p = &backward_f_code;
        *p = _getBits(3);
    }
    _extraBitInformation();
    _extensionAndUserData();
    _updateTemporalReferenceTackingData();
}
