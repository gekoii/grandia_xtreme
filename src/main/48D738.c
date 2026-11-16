/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

extern s32 _scePrintfLinePos;
extern char _scePrintfLineBuf[];

void _sceKputsPutChar(s32 c) {
    s32 n;

    n = _scePrintfLinePos;
    if (n >= 0x7E) {
        _scePrintfLinePos = 0;
        _scePrintfLineBuf[0x7F] = 0;
        kputs(_scePrintfLineBuf, n);
        n = _scePrintfLinePos;
    }
    if (c == '\n') {
        _scePrintfLinePos = 0;
        _scePrintfLineBuf[n] = c;
        _scePrintfLineBuf[n + 1] = 0;
        kputs(_scePrintfLineBuf, n);
        return;
    }
    _scePrintfLinePos = n + 1;
    _scePrintfLineBuf[n] = c;
}
