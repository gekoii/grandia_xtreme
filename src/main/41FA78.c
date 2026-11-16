#include "common.h"
#include "main/n02.h"

typedef struct {
    char s[9];
} S9;

extern void strcat(char *, char *);
extern S9 dbgPathEnemyDir;

void DbgEnemyGbxPathMake(char *arg0, u32 arg1, u8 arg2) {
    char buf[5];
    u16 i;

    buf[0] = (arg1 & 0xF000) >> 12;
    buf[1] = (arg1 & 0xF00) >> 8;
    buf[2] = (arg1 & 0xF0) >> 4;
    buf[3] = arg1 & 0xF;
    for (i = 0; i < 4; i++) {
        if (buf[i] < 10) {
            buf[i] = buf[i] + 0x30;
        } else {
            buf[i] = buf[i] + 0x37;
        }
    }
    buf[4] = 0;
    *(S9 *)arg0 = dbgPathEnemyDir;
    strcat(arg0, buf);
    strcat(arg0, dbgPathSep);
    buf[0] = arg2 + 0x30;
    buf[1] = 0;
    strcat(arg0, buf);
    strcat(arg0, dbgPathGbxExt);
}
