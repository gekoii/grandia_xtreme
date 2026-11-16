#include "common.h"
#include "main/n03.h"
#include "protos/main.h"

s32 FileCmpGetSize(s32 arg0) {
    FileCmpHeader s;
    if (FileCmpReadHeader(&s, arg0) == 0) {
        return 0;
    }
    return s.size;
}
