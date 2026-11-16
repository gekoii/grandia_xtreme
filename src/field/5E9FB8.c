#include "common.h"
#include "protos/field.h"
#include "field/n18.h"


s32 TxtMsgGetWidth(TxtMsgHdr *f, s32 i, s32 size) {
    u8 n = TxtMsgGetLength(f, i);

    return TxtCodesGetWidth(((u16 **)(f->strOfs + (s32)f))[i], n, size);
}
