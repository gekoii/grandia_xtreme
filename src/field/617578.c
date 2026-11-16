#include "common.h"
#include "grxsys.h"



s32 FldSndMakeMapSeName(u16 key, char *buf, u8 idx) {
    u32 i;
    s32 found;
    s32 n;
    s32 t;

    found = -1;
    for (i = 0; i < sysGrxsysPac->mapSe->count; i++) {
        if (key == sysGrxsysPac->mapSe->ents[i].mapNo) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        return 0;
    }
    n = sysGrxsysPac->mapSe->ents[found].banks[idx];
    if (n == -1) {
        return 0;
    }
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
