#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void FxAtkFree(FxAtk *e) {
    if (e->pool) {
        MemFree((void *)e->pool);
        e->pool = 0;
    }
    if (e->buf20) {
        MemFree(e->buf20);
        e->buf20 = NULL;
    }
    if (e->drawList) {
        MemFree((void *)e->drawList);
        e->drawList = 0;
    }
    if (e->work) {
        MemFree(e->work);
        e->work = NULL;
    }
    EnvShadowRemoveCallback(e->shadowCb);
    e->flags = 0;
}
