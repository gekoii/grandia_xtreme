/* cflags: -fno-strength-reduce */
/* without -fno-strength-reduce the loop pass reduces i and off into other registers */
#include "common.h"
#include "gph.h"

void GphTexTableInit(u32 arg0, GphTexTable *arg1) {
    u32 i;
    u32 off;
    u8 c;
    arg1->count = 0;
    arg1->capacity = arg0;
    i = 0;
    if (i < arg0) {
        c = 0xFF;
        off = 0x40;
        do {
            GphTexSlot *e = (GphTexSlot *)((u8 *)arg1 + off); e->flag = c; e->src = 0;
            off += 0x2C0;
            i++;
        } while (i < arg0);
    }
}
