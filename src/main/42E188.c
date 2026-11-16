#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

typedef char *va_list;
#define va_start(AP, LAST)                                                         \
    (AP = ((va_list)__builtin_next_arg(LAST) -                                     \
           (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0)))

void MemSet(void *, s32, s32);

extern u8 dbgPrintfBuf[0x100];

void DbgPrintf(s32 x, s32 y, char *fmt, ...) {
    va_list ap;
    u32 code;
    u32 idx;
    s32 n;
    s32 i;

    idx = dbgTextBuf.count;
    if (idx < 0x7FD) {
        dbgTextBuf.buf[idx] = ((x * (dbgCharW >> 4)) & 0xFFFF) | 0x10000000;
        dbgTextBuf.count = idx + 1;
        dbgTextBuf.buf[idx + 1] = ((y * (dbgCharH >> 4)) & 0xFFFF) | 0x08000000;
        dbgTextBuf.count = idx + 2;
        MemSet(dbgPrintfBuf, 0, 0x100);
        va_start(ap, fmt);
        n = vsprintf(dbgPrintfBuf, fmt, ap);
        if (dbgTextBuf.count + n < 0x7FD) {
            for (i = 0; i < n; i++) {
                if (dbgPrintfBuf[i] < 0x20) {
                    dbgPrintfBuf[i] = ' ';
                }
                if ((s8)dbgPrintfBuf[i] < 0) {
                    dbgPrintfBuf[i] = ' ';
                }
                if (dbgPrintfBuf[i] == '\\') {
                    i++;
                    i += DbgTextParseEscape(&dbgPrintfBuf[i], &code);
                    dbgTextBuf.buf[dbgTextBuf.count++] = code;
                } else {
                    dbgTextBuf.buf[dbgTextBuf.count++] = dbgPrintfBuf[i];
                }
            }
        }
    }
}
