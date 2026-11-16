/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/newlib.h"



s32 fflush(FILE *);
s32 __submore(FILE *);

s32 ungetc(s32 c, FILE *fp) {
    if (c == -1) {
        return -1;
    }
    if (fp->_data == NULL) {
        fp->_data = _impure_ptr;
    }
    do {
        if (fp->_data->__sdidinit == 0) {
            __sinit(fp->_data);
        }
    } while (0);
    fp->_flags &= ~0x20;
    if ((fp->_flags & 4) == 0) {
        if ((fp->_flags & 0x10) == 0) {
            return -1;
        }
        if (fp->_flags & 8) {
            if (fflush(fp)) {
                return -1;
            }
            fp->_flags &= ~8;
            fp->_w = 0;
            fp->_lbfsize = 0;
        }
        fp->_flags |= 4;
    }
    c = (u8)c;
    if (fp->_ub._base != NULL) {
        if (fp->_r >= fp->_ub._size && __submore(fp)) {
            return -1;
        }
        *--fp->_p = c;
        fp->_r++;
        return c;
    }
    if (fp->_bf._base != NULL && fp->_p > fp->_bf._base && fp->_p[-1] == c) {
        fp->_p--;
        fp->_r++;
        return c;
    }
    fp->_ur = fp->_r;
    fp->_up = fp->_p;
    fp->_ub._base = fp->_ubuf;
    fp->_ub._size = sizeof(fp->_ubuf);
    fp->_ubuf[sizeof(fp->_ubuf) - 1] = c;
    fp->_p = &fp->_ubuf[sizeof(fp->_ubuf) - 1];
    fp->_r = 1;
    return c;
}
