#include "common.h"
s32 GsMfifoIsBusy(void) {
    s32 a = *(volatile s32 *)0x10003C00;
    s32 b = *(volatile s32 *)0x1000D010;
    s32 c = *(volatile s32 *)0x10009030;
    s32 ret = 1;
    if (!(a & 0x01F0300F)) {
        ret = b != c;
    }
    return ret;
}
