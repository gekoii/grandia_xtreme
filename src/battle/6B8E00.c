#include "common.h"
#include "gph.h"
#include "chr.h"
#include "protos/main.h"

GphModel *BtlModelCreate(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return NULL;
    }
    return ChrModelCreate((ChrModelData *)arg0);
}
