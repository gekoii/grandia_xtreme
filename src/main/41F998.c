#include "common.h"

typedef struct {
    char s[7];
} S7;

extern void strcat(char *, char *);
extern S7 dbgPathChrDir;
extern char dbgPathChrExt[];

void DbgChrPathMake(char *arg0, u32 arg1) {
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
    *(S7 *)arg0 = dbgPathChrDir;
    strcat(arg0, buf);
    strcat(arg0, dbgPathChrExt);
}
