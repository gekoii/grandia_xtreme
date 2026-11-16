#include "common.h"
#include "main/n03.h"
#include "protos/main.h"


s32 FileGetSizeAligned(s32 arg0) {
    FileFindInfo st;
    s32 n;

    if (SndFsFind((u8 *)arg0, (s32 *)&st) > 0) {
        n = st.size >> 11;
        if (st.size & 0x7FF) {
            n++;
        }
    } else {
        n = 0;
    }
    return n << 11;
}
