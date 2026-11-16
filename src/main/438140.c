#include "common.h"
#include "main/n05.h"

void ChrModelPoolSet(ChrModelPool *arg0) {
    chrModelPool = arg0;
}

ChrModelPool *ChrModelPoolGet(void) {
    return chrModelPool;
}
