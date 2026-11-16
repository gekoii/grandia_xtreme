/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"



s32 _skippedMacroblock(void *, s32 *, void *, s32 *);
s32 _decodeMacroblock(s32 *, s32 *, s32 *, void *, void *, void *);
extern char _sceMpegTooManyMbStr[];

s32 _slice(s32 unused, s32 n) {
    u8 buf[0x20];
    u8 b20[0x10];
    u8 b30[0x10];
    s32 i;
    s32 cnt;
    s32 a;
    s32 b;
    s32 c;
    s32 ret;

    i = 0;
    cnt = 0;
    ret = _sliceA0(n, &i, &cnt, buf);
    if (ret != 0) {
        return ret;
    }
    Fault_Flag = 0;
    for (;;) {
        if (i >= n) {
            return 0;
        }
        _sceMpegMbSlots.slot[_sceMpegMbSlots.cur].skipped = 0;
        if (_sceMpegCheckBdec() == 0) {
            return 2;
        }
        if (cnt == 0) {
            if (_showBits(0x17) == 0 || Fault_Flag != 0) {
                Fault_Flag = 0;
                return 3;
            }
            cnt = _getMacroblockAddressIncrement();
            if (Fault_Flag != 0) {
                Fault_Flag = 0;
                return 1;
            }
        }
        if (i >= n) {
            _sceMpegError(_sceMpegTooManyMbStr);
            return 2;
        }
        if (cnt == 1) {
            if (_decodeMacroblock(&a, &b, &c, buf, b20, b30) == 0) {
                Fault_Flag = 0;
                return 1;
            }
        } else {
            if (_skippedMacroblock(buf, &b, b20, &a) == 0) {
                goto fail;
            }
        }
        if (_motionCompensation(i, cnt, a, b, buf, b20, b30) == 0) {
        fail:
            Fault_Flag = 0;
            return 2;
        }
        if (i != 0) {
            _sceMpegFinishMacroblock(_sceMpegMbSlots.cur ^ 1);
        }
        i++;
        cnt--;
        _sceMpegMbSlots.cur ^= 1;
    }
}
