#include "common.h"
#include "main/n06.h"

void GphPrimListAdd(s32 arg0) {
    GphPrimList *l = gphPrimList;
    s32 n;

    if (l != NULL && arg0 != 0) {
        n = l->count;
        if (n < 0x3FF) {
            l->entries[n].pkt = arg0;
            l->count = n + 1;
            l->entries[n + 1].texId = 0;
        }
    }
}
