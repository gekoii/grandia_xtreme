#include "common.h"
#include "main/n10.h"
s32 SndIsBankLoaded(s32 arg0) {
    s32 idx = sndBankSlot[arg0]; SndIopStatus *t = sndIopStatus; s32 type = t->bankState[arg0]; s16 v = t->strm[idx - 1].secNum; if (type == 2) { if (v == 0) return 0; } return -1;
}
