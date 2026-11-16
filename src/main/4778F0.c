/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"




void _groupOfPicturesHeader(void) {
    s32 *p0 = &drop_flag;
    s32 *p1 = &hour;
    s32 *p2 = &minute;
    s32 *p3 = &_sceMpegGopSec;
    s32 *p4 = &_sceMpegGopFrame;
    s32 *p5 = &closed_gop;
    s32 *p6 = &broken_link;

    _sceMpegCurMp->sys->unkE8 = 0;
    Temporal_Reference_Base = True_Framenum_max + 1;
    Temporal_Reference_GOP_Reset = 1;
    *p0 = _getBits(1);
    *p1 = _getBits(5);
    *p2 = _getBits(6);
    _getBits(1);
    *p3 = _getBits(6);
    *p4 = _getBits(6);
    *p5 = _getBits(1);
    *p6 = _getBits(1);
    _extensionAndUserData();
}
