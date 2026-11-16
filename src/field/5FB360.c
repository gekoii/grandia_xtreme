#include "common.h"
#include "field/n20.h"

ChrObj *ChrGetByNo(s32 n) {
    n--;
    if (n >= 9) {
        return NULL;
    }
    return chrList.chars[n];
}
