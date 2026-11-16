/* Sony PS2 SDK library code (libmpeg; its decoder follows MSSG mpeg2decode): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmpeg.h"
#include "protos/main.h"

s32 _sceMpegDoCallback(sceMpeg *, s32 *);
s32 AddDmacHandler(s32, s32 (*)(s32), s32);
s32 RemoveDmacHandler(s32, s32);
s32 DisableDmac(s32);
s32 EnableDmac(s32);
s32 _sceMpegCscFromHandler(s32);
extern char _sceMpegCscHandlerErrorStr[];

void _sceMpegCscMulti(s32 addr, s32 size) {
    s32 id;
    s32 arg[8];
    s32 n;
    s32 q;

    n = size / 1023 + 1;
    _sceMpegCscChunks = n;
    _sceMpegCscMbLeft = size;
    q = 0xFFC0;
    _sceMpegCscDstAddr = (addr + 0xFFC00) & 0x0FFFFFFF;
    _sceMpegCscError = 0;
    _sceMpegCscChunkDone = 0;
    while (*(volatile s32 *)0x10002010 < 0);
    id = AddDmacHandler(3, _sceMpegCscFromHandler, 0);
    *(volatile s32 *)0x1000E010 = 8;
    EnableDmac(3);
    *(volatile s32 *)0x1000B010 = addr & 0x0FFFFFFF;
    *(volatile s32 *)0x1000B020 = q;
    *(volatile s32 *)0x1000B000 = 0x100;
    *(volatile s32 *)0x10002000 = 0x700003FF;
    arg[0] = 4;
    _sceMpegDoCallback(_sceMpegCurMp, arg);
    while (_sceMpegCscChunkDone < _sceMpegCscChunks);
    if (_sceMpegCscError != 0) {
        _sceMpegError(_sceMpegCscHandlerErrorStr);
    }
    while (*(volatile s32 *)0x10002010 < 0);
    DisableDmac(3);
    RemoveDmacHandler(3, id);
}
