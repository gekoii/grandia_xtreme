#include "common.h"
#include "field/n20.h"

s32 ChrRegister(s16 id, ChrObj *c) {
    s32 i;

    for (i = 0; i < 9; i++) {
        if (chrList.chars[i] == NULL) {
            c->chrId = id;
            chrList.chars[i] = c;
            return i;
        }
    }
    return -1;
}
