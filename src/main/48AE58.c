/* Matches newlib's __swrite (stdio.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct __sFILE {
    u8 pad0[0xC];
    s16 _flags;
    s16 _file;
    u8 pad10[0x44];
    void *_data;
} FILE;


s32 __swrite(void *cookie, const char *buf, s32 n) {
    FILE *fp = (FILE *)cookie;
    s32 w;

    if (fp->_flags & 0x100)
        (void)_lseek_r(fp->_data, fp->_file, 0, 2);
    fp->_flags &= ~0x1000;
    w = _write_r(fp->_data, fp->_file, buf, n);
    return w;
}
