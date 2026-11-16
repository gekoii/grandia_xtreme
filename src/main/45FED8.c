#include "common.h"
#include "protos/main.h"


s32 SndStrmFetch(s32 fd, u8 *buf, s32 n) {
    s32 ret;
    s32 i;
    s32 r;
    s32 m;

    ret = SndStrmFetchStart(fd, buf, n);
    if (ret >= 0) {
        m = -1;
        while (1) {
            for (i = 0; i < 0xA000; i++) {}
            ret = SndStrmFetchPoll(fd);
            if (ret > 0) continue;
            if (ret > m) {
                r = n;
                return r;
            }
            return ret;
        }
    }
    return ret;
}
