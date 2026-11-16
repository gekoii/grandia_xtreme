/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"

typedef struct {
    u8 pad0[0x20];
} Buf;

f32 VuSinAngle(s32);

void FxWallShardsUpdate(BtlUnit *arg0) {
    FxWallShards *p = &fxWallShards[arg0->id];
    Buf buf;
    VuVec4 dir;
    VuVec4 pos;
    FxWallShard *e;
    GphModel *m;
    s32 n;
    s32 i;
    s8 cnt;
    f32 s;

    switch (p->state) {
    case 1:
        if (p->stamp == btlFrameCount) {
            break;
        }
        if (p->kind != 1) {
            GphScreenFadeSetColor(240.0f, 240.0f, 255.0f, 128.0f);
            GphScreenFadeStart(0, 240.0f, 240.0f, 255.0f, 0.0f, 32.0f);
        }
        if (p->pool != 0) {
            MemFree((void *)p->pool);
        }
        n = ChrModelCalcInstanceSize(fxWallShardModel) * 20;
        p->pool = MemAlloc128(n);
        if (p->pool == 0) {
            FxWallShardsFree(p);
            break;
        }
        ChrModelPoolInit(&buf, (void *)p->pool, n);
        if (p->shards != NULL) {
            MemFree(p->shards);
        }
        p->shards = (FxWallShard *)MemAlloc16C(0x190);
        if (p->shards == NULL) {
            FxWallShardsFree(p);
            break;
        }
        ChrModelPoolSet(&buf);
        e = p->shards;
        BtlChrGetPartPos(&pos, arg0, 1);
        i = 20;
        do {
            e->model = BtlModelCreate(fxWallShardModel, 0);
            e->age = 0;
            if (p->kind == 1) {
                e->life = (rand() >> 15) * (1.0f / 65536.0f) * 10.0f + 32.0f;
                BtlVecRandomDirInRange(&dir, 0xC800, 0x4000, (0x5800 - (u16)arg0->rot[1]) & 0xFFFF, 0x5000);
                dir.w = (rand() >> 15) * (1.0f / 65536.0f) * 5.0f + 2.0f;
                s = (rand() >> 15) * (1.0f / 65536.0f) * 0.09f + 0.02f;
                e->model->scale[0] = e->model->scale[1] = e->model->scale[2] = s;
                pos.w = 100.0f;
            } else {
                e->life = (rand() >> 15) * (1.0f / 65536.0f) * 20.0f + 40.0f;
                BtlVecRandomDirInRange(&dir, 0, 0x8000, 0, 0xFFFF);
                dir.w = (rand() >> 15) * (1.0f / 65536.0f) * 5.0f + 15.0f;
                s = (rand() >> 15) * (1.0f / 65536.0f) * 0.7f + 0.3f;
                e->model->scale[0] = e->model->scale[1] = e->model->scale[2] = s;
                pos.w = 160.0f;
            }
            i--;
            e->vel[0] = dir.x * dir.w;
            e->vel[1] = dir.y * dir.w;
            e->vel[2] = dir.z * dir.w;
            e->model->pos[0] = pos.x + dir.x * pos.w;
            e->model->pos[1] = pos.y + dir.y * pos.w;
            e->model->pos[2] = pos.z + dir.z * pos.w;
            e->model->rot[0] = rand() >> 15;
            e->model->rot[1] = rand() >> 15;
            e->model->rot[2] = rand() >> 15;
            e++;
        } while (i > 0);
        ChrModelPoolSet(NULL);
        p->state++;
        break;
    case 2:
        e = p->shards;
        cnt = 0;
        i = 20;
        do {
            if (e->life > 0) {
                cnt++;
                e->vel[1] -= 0.2f;
                m = e->model;
                m->pos[0] += e->vel[0];
                m->pos[1] += e->vel[1];
                m->pos[2] += e->vel[2];
                e->model->rot[0] = e->model->rot[0] + (VuSinAngle(e->age * 0x111) * 3584.0f + 256.0f);
                ChrModelUvScrollUpdate(e->model, 1.0f);
                e->age++;
                if (e->life < e->age) {
                    e->life = 0;
                }
            }
            i--;
            e++;
        } while (i > 0);
        if (cnt == 0) {
            p->stamp = btlFrameCount;
            p->state++;
        }
        break;
    case 3:
        if (p->stamp != btlFrameCount) {
            FxWallShardsFree(p);
        }
        break;
    }
}
