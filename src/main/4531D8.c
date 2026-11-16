#include "common.h"
#include "gph.h"

GphAnimKey *GphAnimGetKey(u32 arg0) {
    GphAnimKey *keys;

    if ((gphCurAnimSet == NULL) || (keys = gphCurAnimSet->keys, (keys == NULL)) || (arg0 >= gphCurAnimSet->keyCount)) {
        return NULL;
    }
    return &keys[arg0];
}
