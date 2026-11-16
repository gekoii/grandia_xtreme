/* rodata: 0x00671498-0x006714A8 */
#include "common.h"

char *strcpy(char *, const char *);
void strcat(char *, char *);

void MapMakeMdtPath(u16 arg0, char *arg1) {
    s8 buf[8];
    u16 i;

    buf[0] = arg0 >> 12;
    buf[1] = (arg0 & 0xF00) >> 8;
    buf[2] = (arg0 & 0xF0) >> 4;
    buf[3] = arg0 & 0xF;
    for (i = 0; i < 4; i++) {
        if (buf[i] < 10) {
            buf[i] += '0';
        } else {
            buf[i] += 'A' - 10;
        }
    }
    buf[4] = 0;
    strcpy(arg1, "FIELD/");
    strcat(arg1, buf);
    strcat(arg1, ".MDT");
}
