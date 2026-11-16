/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"



extern s32 _addBlock(u8 *, s32, s32);
extern s32 func_00475878(u8 *, s32);
extern u8 _sceMpegIntraSkipMbStr[];

void _sceMpegFinishMacroblock(s32 arg0) {
    s32 i;

    if (_sceMpegMbSlots.slot[arg0].predPending != 0) {
        for (i = 0; i < _sceMpegMbSlots.slot[arg0].predCount; i++) {
            _sceMpegMbSlots.slot[arg0].predFuncA[i](&_sceMpegMbSlots.slot[arg0].predArgA[i]);
            _sceMpegMbSlots.slot[arg0].predFuncB[i](&_sceMpegMbSlots.slot[arg0].predArgB[i]);
        }
    }
    if (_sceMpegMbSlots.slot[arg0].intra != 0 && _sceMpegMbSlots.slot[arg0].skipped != 0) {
        _sceMpegError(_sceMpegIntraSkipMbStr);
    }
    if (_sceMpegMbSlots.slot[arg0].intra != 0) {
        func_00475878(_sceMpegMbSlots.slot[arg0].dst, _sceMpegMbSlots.slot[arg0].blockBuf);
    } else if (_sceMpegMbSlots.slot[arg0].skipped != 0) {
        func_00475878(_sceMpegMbSlots.slot[arg0].dst, _sceMpegSprPred);
    } else {
        _addBlock(_sceMpegMbSlots.slot[arg0].dst, _sceMpegSprPred, _sceMpegMbSlots.slot[arg0].blockBuf);
    }
}
