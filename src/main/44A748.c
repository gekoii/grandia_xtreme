#include "common.h"
#include "main/n05.h"
#include "ptcl.h"
#include "protos/main.h"

void MemSet(void *, s32, s32);

s32 PtclGenCreate(s32 buf, PtclGenRequest *d)
{
    s32 n;
    PtclGen **p;
    s32 cnt;
    PtclGen *obj;
    u32 mask;
    s32 i;
    s32 j;
    s32 k;
    u32 *w;
    s16 *h;
    void *src;
    GphModel **ip;
    ChrModelPool *save;
    PtclSprite *v;
    PtclSpriteKey *pr;
    PtclModelPart *e;
    s32 sz;
    GphModel *id;
    u32 *q;
    s32 *zp;
    s32 m;
    s32 c;

    cnt = d->ctrl->groupCount;
    if (buf == 0) {
        return 0;
    }
    p = ptclCurSystem->gens;
    n = ptclCurSystem->genCount;
    while (n > 0) {
        if (*p != NULL && (*p)->texId == d->texId) {
            do {
                n--;
                if (n <= 0) {
                    break;
                }
                p++;
            } while (*p != NULL);
            if (n > 0) {
                goto found;
            }
            break;
        }
        n -= 8;
        p += 8;
    }
    n = ptclCurSystem->genCount;
    p = ptclCurSystem->gens;
    while (n > 0) {
        if (*p == NULL) {
            break;
        }
        n -= 8;
        p += 8;
    }
    if (n > 0) {
        goto found;
    }
    n = ptclCurSystem->genCount;
    p = ptclCurSystem->gens;
    while (n > 0) {
        if (*p == NULL) {
            break;
        }
        n--;
        p++;
    }
    if (n <= 0) {
        return 0;
    }
found:
    *p = (PtclGen *)buf;
    obj = (PtclGen *)buf;
    mask = 0xFFFFFFF0;
    buf += sizeof(PtclGen);
    buf = (buf + 0xF) & mask;
    obj->index = ptclCurSystem->genCount - n;
    obj->texId = d->texId;
    src = d->model;
    if (src == 0) {
        obj->sprites = (PtclSprite *)buf;
        obj->models = NULL;
        buf += cnt * 0x7C0;
        buf = (buf + 0xF) & mask;
        obj->keys = (PtclSpriteKey *)buf;
        buf += cnt * 0x5D0;
        buf = (buf + 0xF) & mask;
        obj->groupLive = (s32 *)buf;
        buf += cnt * 4;
        buf = (buf + 0xF) & mask;
        if (d->ctrl->flags & 1) {
            obj->colorPkt = (u32 *)buf;
            buf += cnt << 9;
            buf = (buf + 0xF) & mask;
            for (i = 0; i < cnt; i++) {
                q = obj->colorPkt + (i << 7);
                *q++ = 0;
                *q++ = 0;
                *q++ = 0;
                *q++ = 0x6E7C0100;
                MemSet(q, 0, 0x1F0);
            }
        } else {
            obj->colorPkt = NULL;
        }
        if (d->animSet != 0) {
            obj->uvPkt = (u32 *)buf;
            buf += cnt << 9;
            buf = (buf + 0xF) & 0xFFFFFFF0;
            obj->animSet = d->animSet;
            obj->anims = (PtclAnimState *)buf;
            for (k = 0; k < cnt; k++) {
                w = obj->uvPkt + (k << 7);
                *w++ = 0x20000000;
                *w++ = 0x10101010;
                *w++ = 0;
                *w++ = 0x757C0080;
                h = (s16 *)w;
                for (m = 0; m < 31; m++) {
                    *h++ = 0;
                    *h++ = 0x400;
                    *h++ = 0;
                    *h++ = 0;
                    *h++ = 0x400;
                    *h++ = 0;
                    *h++ = 0x400;
                    *h++ = 0x400;
                }
            }
        } else {
            obj->animSet = 0;
            obj->uvPkt = NULL;
        }
        v = obj->sprites;
        pr = obj->keys;
        for (c = cnt * 31; c > 0; c--) {
            v->sizeX = v->sizeY = 0.0f;
            pr->life = 0;
            v++;
            pr++;
        }
        zp = obj->groupLive;
        for (c = cnt; c > 0; c--) {
            *zp++ = 0;
        }
    } else {
        obj->modelPool = (ChrModelPool *)buf;
        buf += 0x14;
        buf = (buf + 0xF) & mask;
        sz = ChrModelCalcInstanceSize(src) * cnt;
        ChrModelPoolInit(obj->modelPool, buf, sz);
        buf += sz;
        buf = (buf + 0xF) & mask;
        obj->models = (GphModel **)buf;
        buf += cnt * 4;
        buf = (buf + 0xF) & mask;
        obj->modelParts = (PtclModelPart *)buf;
        save = ChrModelPoolGet();
        ip = obj->models;
        ChrModelPoolSet(obj->modelPool);
        for (j = cnt; j > 0; j--) {
            id = ChrModelCreate(d->model);
            *ip++ = id;
            ChrModelSetPartPrio(id, d->partPrio);
        }
        ChrModelPoolSet(save);
        e = obj->modelParts;
        for (c = cnt; c > 0; c--) {
            e->life = 0;
            e++;
        }
    }
    obj->groupCount = cnt;
    obj->liveCount = 0;
    obj->frame = 0;
    obj->emitCount = 0;
    obj->emitAccum = 0;
    obj->rot[0] = obj->rot[1] = obj->rot[2] = 0;
    /* pos = (0, 0, 0, 1) */
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(obj->pos) : "memory");
    obj->flags = d->ctrl->genFlags;
    obj->blendMode = d->ctrl->blendMode;
    obj->def = d->ctrl->keys[0];
    return 1;
}
