#include "common.h"
#include "main/n02.h"

typedef struct {
    char s[12];
} S12;

extern void strcat(char *, char *);
extern S12 dbgPathPartyPrefix;

void DbgPartyGbxPathMake(char *arg0, s32 arg1, u8 arg2) {
    char buf[2];

    buf[0] = arg1 + 0x31;
    buf[1] = 0;
    *(S12 *)arg0 = dbgPathPartyPrefix;
    strcat(arg0, buf);
    strcat(arg0, dbgPathSep);
    buf[0] = arg2 + 0x30;
    buf[1] = 0;
    strcat(arg0, buf);
    strcat(arg0, dbgPathGbxExt);
}
