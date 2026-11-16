#include "common.h"
#include "field/n20.h"

ChrObj *ChrGetById(u16 id) {
    s32 i;

    for (i = 0; i < 9; i++) {
        if (chrList.chars[i]->chrId == id) {
            return chrList.chars[i];
        }
    }
    return NULL;
}
