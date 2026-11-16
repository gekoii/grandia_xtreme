/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/newlib.h"
#include "protos/main.h"

typedef struct malloc_chunk Chunk;

#define top (((Chunk *)((char *)&__malloc_av_[2] - 8))->fd)

typedef s64 Long;

Long func_00494EB0(Long, Long);

s32 _malloc_trim_r(void *reent, u32 pad) {
    Long top_size;
    Long extra;
    char *current_brk;
    char *new_brk;

    __malloc_lock(reent);
    top_size = top->size & ~3;
    extra = __muldi3(func_00494EB0(top_size - pad - 16 + 4095, 4096) - 1, 4096);
    if (extra < 4096) {
        __malloc_unlock(reent);
        return 0;
    }
    current_brk = _sbrk_r(reent, 0);
    if (current_brk != (char *)top + top_size) {
        __malloc_unlock(reent);
        return 0;
    }
    new_brk = _sbrk_r(reent, -extra);
    if (new_brk == (char *)0xFFFFFFFF) {
        current_brk = _sbrk_r(reent, 0);
        top_size = current_brk - (char *)top;
        if (top_size >= 16) {
            __malloc_current_mallinfo.arena = current_brk - __malloc_sbrk_base;
            top->size = top_size | 1;
        }
        __malloc_unlock(reent);
        return 0;
    }
    top->size = (top_size - extra) | 1;
    __malloc_current_mallinfo.arena -= extra;
    __malloc_unlock(reent);
    return 1;
}
