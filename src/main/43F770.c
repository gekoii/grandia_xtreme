#include "common.h"
#include "protos/main.h"


void GphTexRegisterList(s32 arg0, s32 *arg1) {
    s32 i;
    for (i = 0; i < arg0; i++) {
        GphTexRegisterOwnId(arg1[i]);
    }
}
