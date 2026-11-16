/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct Item48E {
    u8 pad0[0x10];
    s32 flags;
    struct Item48E *self;
    s32 id;
    u8 pad1C[0x24];
} Item48E;

typedef struct {
    s32 counter;
    Item48E *items;
    s32 n;
} Pool48E;

void DIntr(void);
void EIntr(void);

Item48E *_rpc_get_packet(Pool48E *p) {
    s32 i;
    s32 id;
    Item48E *it;

    DIntr();
    it = p->items;
    for (i = 0; i < p->n; i++, it++) {
        if (!(it->flags & 1)) {
            it->flags = (i << 16) | 5;
            if (++p->counter == 1) {
                p->counter++;
                id = 1;
            } else {
                id = p->counter;
            }
            it->self = it;
            it->id = id;
            EIntr();
            return it;
        }
    }
    EIntr();
    return NULL;
}
