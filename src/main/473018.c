/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"



void _sceMpegInitMbBuf(void) {
    _sceMpegSetIpuMp1(1);
    _sceMpegMbSlots.slot[0].predBuf =  _sceMpegSprWork;
    _sceMpegMbSlots.slot[0].blockBuf = (u32)(_sceMpegSprWork + 0x1800);
    _sceMpegMbSlots.slot[1].predBuf =  _sceMpegSprWork + 0x1B00;
    _sceMpegMbSlots.slot[1].blockBuf = (u32)(_sceMpegSprWork + 0x3300);
    _sceMpegMbSlots.cur = 0;
}
