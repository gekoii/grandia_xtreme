/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "party.h"
#include "battle/n28.h"
#include "protos/battle.h"

typedef struct {
    u8 lv[16];
    s8 rate[16];
} LvTbl;

typedef struct {
    s16 id[16];
    s8 rate[16];
} EncTbl;

extern LvTbl *sysDropRankTable;
extern EncTbl *sysDropItemTable;

void BtlEnemyDropRoll(BtlUnit *ctx, f32 add) {
    LvTbl *lt;
    EncTbl *et;
    f32 v;
    f32 r;
    s32 k;
    u8 x;

    lt = &sysDropRankTable[ctx->def->data->dropRank];
    v = ctx->itemRate + add;
    for (k = 15; k > 0; k--) {
        if (lt->lv[k] != 0 && (f32)lt->rate[k - 1] < v) {
            break;
        }
    }
    et = &sysDropItemTable[lt->lv[k]];
    v = (rand() >> 15) * (1.0f / 65536.0f) * 100.0f;
    for (k = 15; k > 0; k--) {
        if (et->id[k] > 0 && (f32)et->rate[k - 1] < v) {
            break;
        }
    }
    ctx->item = et->id[k];
    if (ctx->item > 0) {
        r = (rand() >> 15) * (1.0f / 65536.0f);
        x = ptyItemTable[ctx->item].rank;
        ctx->treasure = x;
        if (r < 0.05f) {
            ctx->treasure = x + 2;
        } else if (r < 0.1f) {
            ctx->treasure = x - 2;
        } else if (r < 0.3f) {
            ctx->treasure = x + 1;
        } else if (r < 0.5f) {
            ctx->treasure = x - 1;
        }
        if (ctx->treasure >= 8) {
            ctx->treasure = 7;
        } else if (ctx->treasure < 0) {
            ctx->treasure = 0;
        }
    } else {
        ctx->treasure = 0;
    }
}
