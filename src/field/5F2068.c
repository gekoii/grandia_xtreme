#include "common.h"
#include "save.h"


s32 PtyCharIsJoined(s16 id) {
    s32 n;
    s32 q;
    if ((u16)id >= 8) return -1;
    n = id + 11;
    q = n / 8;
    n = n - q * 8;
    if ((sysSaveData.systemFlags[q] >> n) & 1) return 0;
    return -1;
}
