#include "common.h"
#include "gph.h"
#include "protos/main.h"

void *GphAnimGetNextKey(s32 arg0) {
    s32 i = arg0 + 1;
    if (i >= gphCurAnimSet->keyCount) {
        i = 0;
    }
    return (void *)GphAnimGetKey(i);
}
