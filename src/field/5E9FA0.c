#include "common.h"
#include "field/n18.h"

u8 TxtMsgGetLength(TxtMsgHdr *f, s32 i) {
    return ((u8 *)(f->lenOfs + (s32)f))[i];
}
