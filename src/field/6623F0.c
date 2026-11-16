#include "common.h"
#include "protos/field.h"

void *memcpy(void *, void *, s32);

s32 EvtMsgGetText(u16 id, u8 *dst, s32 mode) {
    s32 n;
    s32 len;
    u8 *p;
    u8 *q;
    u8 c;
    u8 c2;
    s32 k;

    n = 0;
    p = EvtScnrFind(id);
    if (p != (u8 *)0xFFFFFFFF) {
        q = EvtMsgSkipLeadOps(p);
        c = *q;
        if (c < 3) {
            *dst = 0;
            return 0;
        }
        p = q;
        len = 0;
        for (;;) {
            if (mode == 0) {
                if (c >= 0x20) {
                    n++;
                }
            } else {
                if ((u8)(c - 0x20) <= 0xCF) {
                    n++;
                }
            }
            q++;
            if (c == 0x17) {
                c2 = *q;
                if (mode == 0) {
                    q++;
                    if (c2 == 0) {
                        break;
                    }
                } else {
                    if (c2 == 0) {
                        break;
                    }
                    if (c2 == 0x80) {
                        q += 2;
                    } else {
                        q++;
                    }
                }
            }
            c = *q;
            len++;
        }
        if (n > 0) {
            k = len - 1;
            if (mode == 0) {
                k = len - 2;
            }
            memcpy(dst, p + 4, k - 1);
            dst[k] = 0;
        } else {
            *dst = 0;
        }
    } else {
        *dst = 0;
    }
    return n;
}
