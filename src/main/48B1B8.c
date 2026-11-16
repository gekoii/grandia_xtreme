/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 *base;
    s32 size;
} sbuf;

typedef struct {
    u8 *p;
    u8 pad4[0x30 - 4];
    sbuf ub;
    u8 pad38[0x40 - 0x38];
    u8 ubuf[3];
    u8 pad43[0x54 - 0x43];
    void *data;
} FILE_;

extern void memcpy(u8 *, u8 *, s32);
extern u8 *_malloc_r(void *, s32);
extern u8 *_realloc_r(void *, u8 *, s32);

s32 __submore(FILE_ *fp) {
    s32 i;
    u8 *p;

    if (fp->ub.base == fp->ubuf) {
        if ((p = _malloc_r(fp->data, 1024)) == NULL) {
            return -1;
        }
        fp->ub.base = p;
        fp->ub.size = 1024;
        p += 1024 - sizeof(fp->ubuf);
        for (i = sizeof(fp->ubuf); --i >= 0;) {
            p[i] = fp->ubuf[i];
        }
        fp->p = p;
        return 0;
    }
    i = fp->ub.size;
    p = _realloc_r(fp->data, fp->ub.base, i << 1);
    if (p == NULL) {
        return -1;
    }
    memcpy(p + i, p, i);
    fp->p = p + i;
    fp->ub.base = p;
    fp->ub.size = i << 1;
    return 0;
}
