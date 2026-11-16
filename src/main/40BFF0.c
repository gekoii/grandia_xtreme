#include "common.h"
#include "protos/main.h"


s32 UtilLoadFile(s32 arg0, s32 arg1) {
    s32 n;

    n = FileGetSizeAligned(arg0);
    if (n <= 0) {
        return 0;
    }
    if (arg1 == 0) {
        arg1 = MemAlloc128(n);
    }
    FileRead(arg0, arg1);
    return arg1;
}
