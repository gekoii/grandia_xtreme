#include "common.h"
#include "protos/main.h"

/* Saved graphics context: packet buffer, light list, texture table and
 * draw list (GphGetContext fills it, GphSetContext restores the non-zero
 * entries). */
typedef struct GphContext {
    /* 0x0 */ s32 packetBuf;
    /* 0x4 */ struct GphLightList *lightList;
    /* 0x8 */ struct GphTexTable *texTable;
    /* 0xC */ s32 drawList;
} GphContext;

void GphSetContext(void *arg0) {
    GphContext *ctx = arg0;

    if (ctx->packetBuf != 0) {
        GsSetPacketBuf(ctx->packetBuf);
    }
    if (ctx->lightList != 0) {
        GphLightListSet(ctx->lightList);
    }
    if (ctx->texTable != 0) {
        GphTexTableSelect(ctx->texTable);
    }
    if (ctx->drawList != 0) {
        GphDrawListSet(ctx->drawList);
    }
}

void GphGetContext(void *arg0) {
    GphContext *ctx = arg0;

    ctx->packetBuf = GsGetPacketBuf();
    ctx->lightList = GphLightListGetCurrent();
    ctx->texTable = GphTexTableGetCurrent();
    ctx->drawList = GphDrawListGetCurrent();
}
