/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"


void _copyrightExtension(void) {
    s32 *p;

    p = &copyright_flag;
    *p = _getBits(1);
    p = &copyright_identifier;
    *p = _getBits(8);
    p = &original_or_copy;
    *p = _getBits(1);
    _getBits(7);
    _getBits(1);
    p = &copyright_number_1;
    *p = _getBits(0x14);
    _getBits(1);
    p = &copyright_number_2;
    *p = _getBits(0x16);
    _getBits(1);
    p = &copyright_number_3;
    *p = _getBits(0x16);
}
