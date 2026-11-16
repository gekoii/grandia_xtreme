#include "common.h"
#include "protos/main.h"
#include "chr.h"

typedef struct {
    u8 pad0[0x60];
    s32 w60;
} Item;


s32 ChrPackBind(ChrPackBinding *d, ChrPack *s) {
    s32 i;
    s32 t;

    d->pack = s;
    t = s->modelPac;
    d->modelPac = t;
    FilePacModelBindTextures(t);
    for (i = 0; i < 8; i++) {
        if (s->items[i] != 0) {
            d->faces.ids[i] = ((Item *)s->items[i])->w60;
        } else {
            d->faces.ids[i] = -1;
        }
    }
    return 0;
}
