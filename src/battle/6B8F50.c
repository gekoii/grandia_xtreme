#include "common.h"
#include "protos/battle.h"
#include "main/n05.h"
#include "battle/n32.h"

GphModel *BtlModelCreateWithPool(s32 id, s32 *bufp) {
    ChrModelPool st;
    GphModel *ret;

    if (id == 0) {
        return NULL;
    }
    BtlModelPoolReserve(id, 1);
    if ((*bufp = BtlModelPoolAlloc(&st)) == 0) {
        return NULL;
    }
    ret = BtlModelCreate(id, 0);
    ChrModelPoolSet(NULL);
    return ret;
}
