/* rodata: 0x004A3630-0x004A3640 */
#include "common.h"

char *strcpy(char *, const char *);
extern char *strcat(char *, const char *);

void DbgSePathMake(char *arg0, u32 arg1) {
    char buf[5];
    strcpy(arg0, "MUSIC/");
    buf[0] = arg1 / 1000; arg1 -= buf[0] * 1000;
    buf[1] = arg1 / 100; arg1 -= buf[1] * 100;
    buf[2] = arg1 / 10; arg1 -= buf[2] * 10;
    buf[0] += 0x30;
    buf[1] += 0x30;
    buf[2] += 0x30;
    buf[3] = arg1 + 0x30;
    buf[4] = 0;
    strcat(arg0, buf);
    strcat(arg0, ".SE");
}
