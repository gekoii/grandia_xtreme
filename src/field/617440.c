#include "common.h"

s32 FldSndMakePartySeName(u8 id, char *buf, s8 bank) {
    s32 n;
    s32 t;

    if (bank == 0) {
        return -1;
    }
    n = 0x1965;
    n = (bank - 2) * 3 + (id + n);
    buf[0] = 'M';
    buf[1] = 'U';
    buf[2] = 'S';
    buf[3] = 'I';
    buf[4] = 'C';
    buf[5] = '/';
    buf[6] = n / 1000 + '0';
    t = n - n / 1000 * 1000;
    t = t / 100;
    buf[7] = t + '0';
    t = n - n / 1000 * 1000;
    t = t - t / 100 * 100;
    buf[8] = t / 10 + '0';
    t = n - n / 1000 * 1000;
    t = t - t / 100 * 100;
    t = t - t / 10 * 10;
    buf[9] = t + '0';
    buf[10] = '.';
    buf[11] = 'S';
    buf[12] = 'E';
    buf[13] = 0;
    return n;
}
