/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 x0;
    s32 count;
    void *data;
} H80F8;

extern void func_0048281C(void *, s32, s32);
extern H80F8 *_malloc_r(s32, s32);

H80F8 *__sfmoreglue(s32 arg0, s32 arg1) {
    s32 size;
    H80F8 *h;

    size = arg1 * 0x58;
    h = _malloc_r(arg0, size + 0xC);
    if (h == NULL) {
        return NULL;
    }
    h->count = arg1;
    h->x0 = 0;
    h->data = (u8 *)h + 0xC;
    func_0048281C(h->data, 0, size);
    return h;
}
