#include "common.h"
#include "main/n08.h"

void sceDmaSendN(sceDmaChan *, u32, s32);

void GsVif0DmaSendN(void *arg0, s32 arg1) {
    u32 a = (u32)arg0;
    if (a >= 0x70000000) {
        a = (a - 0x70000000) | 0x80000000;
    }
    sceDmaSendN(gsDmaChanVif0, a, arg1);
}
