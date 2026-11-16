#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/n22.h"

typedef struct {
    u8 lv[16];
    s8 rate[16];
} LvTbl;

typedef struct {
    s16 id[16];
    s8 rate[16];
} EncTbl;

typedef struct {
    u8 b[6];
} Col6;

typedef struct {
    s16 id;
    s16 lv;
} Out;

extern LvTbl *sysDropRankTable;
extern EncTbl *sysDropItemTable;
extern s32 amapItemLogPos;
extern s16 amapItemRollLogId[];
extern Col6 amapItemRollLog[];

void AmapRollItemsByRank(s32 count, s32 area, FldMapSet *unused, Out *out) {
    s32 k;
    s32 n;
    s16 id;
    s32 r;
    s32 j;
    s32 m;
    u8 lv;

    k = 0;
    n = 0;
    id = 0;
    for (;;) {
        amapItemLogPos &= 0x1F;
        amapItemRollLog[amapItemLogPos].b[0] = area;
        if (k >= count) break;
        r = VuRandInt(100) + 1;
        for (j = 0; j < 16; j++) {
            if (sysDropRankTable[area].rate[j] >= r) {
                amapItemRollLog[amapItemLogPos].b[1] = r;
                amapItemRollLog[amapItemLogPos].b[2] = j;
                lv = sysDropRankTable[area].lv[j];
                r = VuRandInt(100) + 1;
                for (m = 0; m < 16; m++) {
                    if (sysDropItemTable[lv].rate[m] >= r) {
                        id = sysDropItemTable[lv].id[m];
                        amapItemRollLog[amapItemLogPos].b[3] = lv;
                        amapItemRollLog[amapItemLogPos].b[4] = r;
                        amapItemRollLog[amapItemLogPos].b[5] = m;
                        amapItemRollLogId[amapItemLogPos] = id;
                        break;
                    }
                }
                break;
            }
        }
        if (id != 0) {
            out[n].id = id;
            out[n].lv = ptyItemTable[id].rank;
            amapItemLogPos++;
            k++;
            n++;
        }
    }
}
