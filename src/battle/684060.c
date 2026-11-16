/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n28.h"
#include "gph.h"

typedef struct {
    u8 pad0[4];
    s32 unk4;
} Res;


#define RND() ((rand() >> 15) * (1.0f / 65536.0f))

void BtlDropObjUpdate(void) {
    BtlDropObj *e;
    BtlDropCoin *q;
    Res *h;
    Res *h2;
    GphModel *o;
    s32 i;
    s32 n;
    u8 buf[0x20];

    e = btlDropObjs;
    for (i = 0; i < 12; i++, e++) {
        if (e->kind <= 0) {
            continue;
        }
        if (e->kind < 3) {
            switch (e->state) {
            case 0:
                if (e->frameStamp == btlFrameCount) {
                    break;
                }
                if (e->modelBuf != 0) {
                    MemFree(e->modelBuf);
                }
                h = SysCommonModelGet(0x4E);
                BtlModelPoolReserve(h, e->coinCount);
                e->modelBuf = BtlModelPoolAlloc(buf);
                if (e->modelBuf == 0) {
                    BtlDropObjFree(e);
                    continue;
                }
                n = e->coinCount;
                q = e->coins;
                while (n > 0) {
                    n--;
                    o = BtlModelCreate(h, 0);
                    q->model = o;
                    o->pos[0] = e->x;
                    o->pos[1] = e->y;
                    o->pos[2] = e->z;
                    q->vx = (RND() - 0.5f) * 4.0f;
                    q->vy = RND() * 10.0f;
                    q->vz = (RND() - 0.5f) * 4.0f;
                    q->model->rot[0] = rand() >> 15;
                    q->model->rot[1] = rand() >> 15;
                    q->rotVelX = 8192.0f - RND() * 16384.0f;
                    q->rotVelY = 8192.0f - RND() * 16384.0f;
                    q->bounced = 0;
                    q++;
                }
                ChrModelPoolSet(0);
                e->drawList = MemAlloc32(GphDrawListSize(h->unk4) * e->coinCount);
                if (e->drawList == 0) {
                    BtlDropObjFree(e);
                    continue;
                }
                GphDrawListInit(h->unk4 * e->coinCount, e->drawList);
                GphDrawListSet(e->drawList);
                GphDrawListClear(NULL);
                e->timer = 0;
                e->alpha = 1.0f;
                e->state++;
                break;
            case 1:
                BtlDropCoinsUpdate(e);
                if (e->timer >= 0xE0) {
                    e->alpha -= 0.03125f;
                    if (e->alpha <= 0.0f) {
                        e->frameStamp = btlFrameCount;
                        e->state++;
                    }
                }
                break;
            case 2:
                if (e->frameStamp != btlFrameCount) {
                    BtlDropObjFree(e);
                }
                return;
            }
        } else {
            switch (e->state) {
            case 0:
                if (e->frameStamp == btlFrameCount) {
                    break;
                }
                if (e->modelBuf != 0) {
                    MemFree(e->modelBuf);
                }
                h2 = SysCommonModelGet(e->kind);
                e->model = BtlModelCreateWithPool(h2, &e->modelBuf);
                if (e->model == NULL) {
                    BtlDropObjFree(e);
                    continue;
                }
                e->drawList = MemAlloc32(GphDrawListSize(h2->unk4));
                if (e->drawList == 0) {
                    BtlDropObjFree(e);
                    continue;
                }
                GphDrawListInit(h2->unk4, e->drawList);
                GphDrawListSet(e->drawList);
                GphDrawListClear(NULL);
                e->alpha = 1.0f;
                e->model->pos[0] = e->x;
                e->model->pos[1] = e->y;
                e->model->pos[2] = e->z;
                e->timer = 0;
                e->vy = 10.0f;
                e->state++;
                break;
            case 1:
                e->vy -= 1.0f;
                e->y += e->vy;
                if (e->y <= 0.1f) {
                    e->vy *= -0.3f;
                    if (e->vy <= 1.0f) {
                        e->y = 0.1f;
                        e->vy = 0.0f;
                    } else {
                        e->vx *= 0.95f;
                        e->vz *= 0.95f;
                        e->y = 0.2f - e->y;
                    }
                }
                if (0.1f < e->y) {
                    e->x += e->vx;
                    e->z += e->vz;
                }
                o = e->model;
                o->pos[0] = e->x;
                o->pos[1] = e->y;
                o->pos[2] = e->z;
                ChrModelUpdate(o, 1.0f);
                if (e->timer >= 0xE0) {
                    e->alpha -= 0.03125f;
                    if (e->alpha <= 0.0f) {
                        e->frameStamp = btlFrameCount;
                        e->state++;
                        e->model = NULL;
                    }
                }
                break;
            case 2:
                if (e->frameStamp != btlFrameCount) {
                    BtlDropObjFree(e);
                }
                return;
            }
        }
        e->timer++;
    }
}
