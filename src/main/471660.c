/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"



s32 _sceMpegIdToStrType(u32 *arg0, s32 *arg1, u64 arg2) {
    SceMpegStrType *e;
    s32 found;
    u32 i;
    u64 m;
    u64 r;
    u64 n;

    e = _sceMpegStrTypeTable;
    found = 0;
    for (i = 0; i < 10 && found == 0; i++, e++) {
        switch (e->mask) {
        case 0xFFFFFFFFFFULL:
            m = arg2 & 0xFFFFFF0000ULL;
            if (m == e->pattern) {
                *arg0 = i;
                found = 1;
                *arg1 = arg2 & 0xFFFF;
            }
            break;
        case 0xFFFF000000ULL:
            switch (e->pattern) {
            case 0xBD20000000ULL:
                m = arg2 & 0xFFE0000000ULL;
                n = 0x1F;
                break;
            case 0xBD80000000ULL:
            case 0xBD88000000ULL:
            case 0xBD90000000ULL:
            case 0xBDA0000000ULL:
                m = arg2 & 0xFFF8000000ULL;
                n = 7;
                break;
            default:
                m = arg2 & 0xFFFF000000ULL;
                n = 0;
                break;
            }
            if (m == e->pattern) {
                *arg0 = i;
                found = 1;
                *arg1 = (arg2 >> 24) & n;
            }
            break;
        case 0xFF00000000ULL:
            if (e->pattern == 0xE000000000ULL) {
                m = arg2 & 0xF000000000ULL;
                n = 0xF;
            } else if (e->pattern == 0xC000000000ULL) {
                m = arg2 & 0xE000000000ULL;
                n = 0x1F;
            } else {
                m = arg2 & e->mask;
                n = 0;
            }
            if (m == e->pattern) {
                *arg0 = i;
                found = 1;
                *arg1 = (arg2 >> 32) & n;
            }
            break;
        }
    }
    return found;
}
