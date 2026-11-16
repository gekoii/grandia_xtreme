#include "common.h"
#include "main/n06.h"
#include "protos/main.h"

void GphPrimListDraw(GphPrimList *arg0) {
    s32 n = arg0->count;
    GphPrimEntry *e = arg0->entries;
    s32 i;

    for (i = 0; i < n; i++) {
        if (e[i].texId != 0) {
            GphTexBind(e[i].texId);
        }
        GsGifPacketSend(e[i].pkt);
        GsFromSprDmaWait();
    }
}
