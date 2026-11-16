#include "common.h"
#include "main/n08.h"

void sceDmaSend(sceDmaChan *, u32);

void GsVif0DmaSend(u32 arg0) {
    u32 a = arg0; if (a > 0x6FFFFFFF) { a = (a - 0x70000000) | 0x80000000; } sceDmaSend(gsDmaChanVif0, a);
}
