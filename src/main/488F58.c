/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/newlib.h"

typedef struct malloc_chunk *mchunkptr;


#define top (__malloc_av_[2])
#define initial_top ((mchunkptr)__malloc_av_)
#define top_pad __malloc_top_pad
#define sbrk_base __malloc_sbrk_base
#define max_sbrked_mem __malloc_max_sbrked_mem
#define max_total_mem __malloc_max_total_mem
#define sbrked_mem __malloc_current_mallinfo.arena

void malloc_extend_top(void *reent_ptr, u32 nb) {
    char *brk;
    u32 front_misalign;
    u32 correction;
    char *new_brk;
    u32 top_size;
    mchunkptr old_top = top;
    u32 old_top_size = old_top->size & ~3;
    char *old_end = (char *)old_top + old_top_size;
    u32 sbrk_size = nb + top_pad + 16;
    u64 pagesz = 4096;

    if (sbrk_base != (char *)-1) {
        sbrk_size = (sbrk_size + (pagesz - 1)) & ~(pagesz - 1);
    }

    brk = _sbrk_r(reent_ptr, sbrk_size);

    if (brk == (char *)-1 || (brk < old_end && old_top != initial_top)) {
        return;
    }

    sbrked_mem += sbrk_size;

    if (brk == old_end) {
        top_size = sbrk_size + old_top_size;
        top->size = top_size | 1;
    } else {
        if (sbrk_base == (char *)-1) {
            sbrk_base = brk;
        } else {
            sbrked_mem += brk - old_end;
        }

        front_misalign = (u32)(brk + 8) & 15;
        if (front_misalign > 0) {
            correction = 16 - front_misalign;
            brk += correction;
        } else {
            correction = 0;
        }

        correction += pagesz - ((u32)(brk + sbrk_size) & (pagesz - 1));

        new_brk = _sbrk_r(reent_ptr, correction);
        if (new_brk == (char *)-1) {
            return;
        }

        sbrked_mem += correction;

        top = (mchunkptr)brk;
        top_size = new_brk - brk + correction;
        top->size = top_size | 1;

        if (old_top != initial_top) {
            if (old_top_size < 16) {
                top->size = 1;
                return;
            }
            old_top_size = (old_top_size - 12) & ~15;
            old_top->size = (old_top->size & 1) | old_top_size;
            ((mchunkptr)((char *)old_top + old_top_size))->size = 5;
            ((mchunkptr)((char *)old_top + old_top_size + 4))->size = 5;
            if (old_top_size >= 16) {
                _free_r(reent_ptr, (char *)old_top + 8);
            }
        }
    }

    if ((u64)sbrked_mem > max_sbrked_mem) {
        max_sbrked_mem = sbrked_mem;
    }
    if ((u64)sbrked_mem > max_total_mem) {
        max_total_mem = sbrked_mem;
    }
}
