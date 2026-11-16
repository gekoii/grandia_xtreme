#include "common.h"
#include "snd.h"

u32 getFIFOindex(ViBuf *arg0, u32 arg1) {
    if (arg1 == ((arg0->n * 16 + (s32)arg0->tag + 16) & 0x0FFFFFFF)) {
        return 0;
    }
    return (arg1 - (u32)arg0->data) >> 11;
}
