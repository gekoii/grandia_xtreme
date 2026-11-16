/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/newlib.h"




void __smakebuf(FILE *);

s32 __swsetup(FILE *fp) {
    if (fp->_data == NULL) {
        fp->_data = _impure_ptr;
    }
    do {
        if (!fp->_data->__sdidinit) {
            __sinit(fp->_data);
        }
    } while (0);
    if ((fp->_flags & 8) == 0) {
        if ((fp->_flags & 0x10) == 0) {
            return -1;
        }
        if (fp->_flags & 4) {
            if (fp->_ub._base != NULL) {
                if (fp->_ub._base != fp->_ubuf) {
                    _free_r(fp->_data, fp->_ub._base);
                }
                fp->_ub._base = NULL;
            }
            fp->_flags &= ~(4 | 0x20);
            fp->_r = 0;
            fp->_p = fp->_bf._base;
        }
        fp->_flags |= 8;
    }
    if (fp->_bf._base == NULL) {
        __smakebuf(fp);
    }
    if (fp->_flags & 1) {
        fp->_w = 0;
        fp->_lbfsize = -fp->_bf._size;
    } else {
        fp->_w = (fp->_flags & 2) ? 0 : fp->_bf._size;
    }
    return 0;
}
