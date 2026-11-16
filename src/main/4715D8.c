/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"



u64 _sceMpegStrTypeToId(u32 i, u64 v) {
    u64 r;
    u32 sh;

    r = 0;
    sh = 0;
    if (i < 10) {
        switch (_sceMpegStrTypeTable[i].mask) {
        case 0xFFFFFFFFFFULL:
            sh = 0;
            break;
        case 0xFFFF000000ULL:
            sh = 24;
            break;
        case 0xFF00000000ULL:
            sh = 32;
            break;
        }
        do {
            do {
                r = _sceMpegStrTypeTable[i].pattern | (v << sh);
            } while (0);
        } while (0);
    }
    return r;
}
