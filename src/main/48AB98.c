/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/newlib.h"




extern s32 fflush(FILE *);
extern s32 _fwalk(struct _reent *, s32 (*)(FILE *));
extern void __smakebuf(FILE *);

s32 __srefill(FILE *fp) {
    do {
        if (fp->_data == 0)
            fp->_data = _impure_ptr;
        if (!fp->_data->__sdidinit)
            __sinit(fp->_data);
    } while (0);

    fp->_r = 0;
    if (fp->_flags & 0x20)
        return -1;
    if ((fp->_flags & 0x4) == 0) {
        if ((fp->_flags & 0x10) == 0)
            return -1;
        if (fp->_flags & 0x8) {
            if (fflush(fp))
                return -1;
            fp->_flags &= ~0x8;
            fp->_w = 0;
            fp->_lbfsize = 0;
        }
        fp->_flags |= 0x4;
    } else {
        if (fp->_ub._base != NULL) {
            if (fp->_ub._base != fp->_ubuf)
                _free_r(fp->_data, fp->_ub._base);
            fp->_ub._base = NULL;
            if ((fp->_r = fp->_ur) != 0) {
                fp->_p = fp->_up;
                return 0;
            }
        }
    }

    if (fp->_bf._base == NULL)
        __smakebuf(fp);

    if (fp->_flags & (0x1 | 0x2))
        (void)_fwalk(fp->_data, lflush);
    fp->_p = fp->_bf._base;
    fp->_r = (*fp->_read)(fp->_cookie, (char *)fp->_p, fp->_bf._size);
    fp->_flags &= ~0x2000;
    if (fp->_r <= 0) {
        if (fp->_r == 0)
            fp->_flags |= 0x20;
        else {
            fp->_r = 0;
            fp->_flags |= 0x40;
        }
        return -1;
    }
    return 0;
}
