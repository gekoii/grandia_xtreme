#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

s32 strlen(u8 *);
extern u8 dbgPutsBuf[];

void DbgPuts(s32 x, s32 y, u8 *str) {
    s32 len;
    s32 i;
    s32 n;
    s32 tmp;

    len = strlen(str);
    n = dbgTextBuf.count;
    if ((u32)(n + len) < 0x7FD) {
        dbgTextBuf.buf[n] = ((x * (dbgCharW >> 4)) & 0xFFFF) | 0x10000000;
        dbgTextBuf.count = n + 1;
        dbgTextBuf.buf[n + 1] = ((y * (dbgCharH >> 4)) & 0xFFFF) | 0x08000000;
        dbgTextBuf.count = n + 2;
        for (i = 0; i < len; i++) {
            dbgPutsBuf[i] = str[i];
            if (dbgPutsBuf[i] < 0x20) {
                dbgPutsBuf[i] = 0x20;
            }
            if ((s8)dbgPutsBuf[i] < 0) {
                dbgPutsBuf[i] = 0x20;
            }
            if (dbgPutsBuf[i] == '\\') {
                i++;
                i += DbgTextParseEscape(&dbgPutsBuf[i], &tmp);
                dbgTextBuf.buf[dbgTextBuf.count++] = tmp;
            } else {
                dbgTextBuf.buf[dbgTextBuf.count++] = dbgPutsBuf[i];
            }
        }
    }
}
