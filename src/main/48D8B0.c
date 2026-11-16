/* rodata: 0x004A8E00-0x004A8E30 */
/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

extern s32 _scePrintfDoubleToInt(s32);
extern void _sceSioPrintf(const char *, ...);
extern s32 func_00494758(double);
extern void (*_scePrintfPutchar)(s32);

void _scePrintfDouble(double x) {
    s32 e;
    s32 n;

    e = 0;
    if (x < 0.0) {
        x = 0.0 - x;
        _scePrintfPutchar('-');
    }
    if (x < 0.1) {
        while (x < 0.1) {
            e--;
            x = x * 10.0;
        }
    } else if (x >= 1.0) {
        while (x >= 1.0) {
            e++;
            x = x / 10.0;
        }
    }
    n = _scePrintfDoubleToInt(func_00494758(x * 1000000.0));
    _sceSioPrintf("0.%d", n);
    if (e >= 0) {
        _sceSioPrintf("e+%d", e);
    } else {
        _sceSioPrintf("e%d", e);
    }
}
