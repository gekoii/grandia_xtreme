#include "common.h"
#include "party.h"
#include "battle/misc.h"
#include "protos/battle.h"

extern char dbgBtlCfgStrEnd[];
extern char dbgBtlCfgStrDmgRnd[];
extern char dbgBtlCfgFmtInt[];
extern char dbgBtlCfgStrNoDeath[];
extern char dbgBtlCfgStrNoMpsp[];
extern char dbgBtlCfgStrDbgPcai[];
extern char dbgBtlCfgStrMgcGbx[];
extern char dbgBtlCfgStrAi[];
extern char dbgBtlCfgStrAiMgcNo[];
extern char dbgBtlCfgStrAiMgcCmb[];
extern char dbgBtlCfgStrPcai1[];
extern char dbgBtlCfgStrPcai2[];
extern char dbgBtlCfgStrMcAi[];
extern char dbgBtlCfgStrMcAtk[];
extern char dbgBtlCfgStrStop[];
extern char dbgBtlCfgStrSpP[];
extern char dbgBtlCfgFmtInt6[];
extern char dbgBtlCfgStrSpC2[];
extern char dbgBtlCfgFmtInt8[];
extern char dbgBtlCfgStrSpC3[];
extern char dbgBtlCfgFmtInt7[];
extern char dbgBtlCfgStrSkl[];

void DbgBtlCfgParse(char *arg0) {
    s32 c[2];
    s32 lv[5];
    s32 a[8];
    s32 b[7];
    s32 n;
    char *p;
    s32 i;

    p = arg0;
    n = 0;
    while (!UtilMatchKeyword(&p, dbgBtlCfgStrEnd, 0)) {
        if (UtilMatchKeyword(&p, dbgBtlCfgStrDmgRnd, 0)) {
            s32 v;
            sscanf(p, dbgBtlCfgFmtInt, &v);
            if (v == 0) {
                btlDebugCfg.flags |= BTL_DBG_DMG_RND;
            }
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrNoDeath, 0)) {
            s32 v;
            sscanf(p, dbgBtlCfgFmtInt, &v);
            if (v != 0) {
                btlDebugCfg.flags |= BTL_DBG_NO_DEATH;
            }
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrNoMpsp, 0)) {
            s32 v;
            sscanf(p, dbgBtlCfgFmtInt, &v);
            if (v != 0) {
                btlDebugCfg.flags |= BTL_DBG_NO_MPSP;
            }
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrDbgPcai, 0)) {
            s32 v;
            sscanf(p, dbgBtlCfgFmtInt, &v);
            if (v != 0) {
                btlDebugCfg.flags |= BTL_DBG_AI;
            }
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrMgcGbx, 0)) {
            s32 v;
            sscanf(p, dbgBtlCfgFmtInt, &v);
            if (v != 0) {
                btlDebugCfg.flags |= BTL_DBG_MGC_GBX;
            }
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrAi, 0)) {
            s32 v;
            sscanf(p, dbgBtlCfgFmtInt, &v);
            btlAutoTactics = v;
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrAiMgcNo, 0)) {
            s32 v;
            sscanf(p, dbgBtlCfgFmtInt, &v);
            btlDebugCfg.mgcNo = v;
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrAiMgcCmb, 0)) {
            s32 v;
            sscanf(p, dbgBtlCfgFmtInt, &v);
            btlDebugCfg.mgcCmb = v;
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrPcai1, 0)) {
            sscanf(p, dbgBtlCfgFmtInt, &c[0]);
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrPcai2, 0)) {
            sscanf(p, dbgBtlCfgFmtInt, &c[1]);
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrMcAi, 0)) {
            s32 v;
            sscanf(p, dbgBtlCfgFmtInt, &v);
            btlDebugCfg.aiNo = v;
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrMcAtk, 0)) {
            s32 v;
            sscanf(p, dbgBtlCfgFmtInt, &v);
            btlDebugCfg.atkNo = v;
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrStop, 0)) {
            s32 bit;
            for (bit = 0; bit < 16; bit++) {
                if (UtilReadInt(&p, 0)) {
                    btlDebugCfg.ipStopMask |= 1 << bit;
                }
            }
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrSpP, 1)) {
            s32 k;
            s32 i;
            sscanf(p, dbgBtlCfgFmtInt6, &k, &lv[0], &lv[1], &lv[2], &lv[3], &lv[4]);
            k--;
            for (i = 0; i < 5; i++) {
                ptyPartyData->moveLevels[k * 5 + i].level = lv[i];
            }
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrSpC2, 0)) {
            s32 i, j;
            sscanf(p, dbgBtlCfgFmtInt8, &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7]);
            for (i = 0; i < 8; i++) {
                PtyMoveLevel *s = &ptyPartyData->moveLevels[PTY_MOVE_LEVEL_MOVES];
                for (j = 0; j < 50; j++, s++) {
                    if (s->moveId == i + 0xDD) {
                        s->level = a[i];
                    }
                }
            }
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrSpC3, 0)) {
            s32 i, j;
            sscanf(p, dbgBtlCfgFmtInt7, &b[0], &b[1], &b[2], &b[3], &b[4], &b[5], &b[6]);
            for (i = 0; i < 7; i++) {
                PtyMoveLevel *s = &ptyPartyData->moveLevels[PTY_MOVE_LEVEL_MOVES];
                for (j = 0; j < 50; j++, s++) {
                    if (s->moveId == i + 0xE7) {
                        s->level = b[i];
                    }
                }
            }
        } else if (UtilMatchKeyword(&p, dbgBtlCfgStrSkl, 1)) {
            s32 cat = UtilReadInt(&p, 0) - 1;
            s32 i;
            for (i = 0; i < 15; i++) {
                s32 id = UtilReadInt(&p, 0);
                s32 lvl = UtilReadInt(&p, 0);
                if (id > 0 && n < 100) {
                    PtySkill *sk = &ptyPartyData->skills[n];
                    s32 m;
                    sk->id = id;
                    sk->owner = cat;
                    if (lvl > ptySkillTable[id].maxLevel) lvl = ptySkillTable[id].maxLevel;
                    m = lvl;
                    sk->level = m;
                    if (m > 0) {
                        sk->exp = ptySkillTable[id].expNeeded[m - 1];
                    } else {
                        sk->exp = 0;
                    }
                    n++;
                }
            }
        }
        while (*p != '\n') {
            p++;
        }
        p++;
    }
    for (i = 0; i < 4; i++) {
        btlTactics[i][0] = c[0];
        btlTactics[i][1] = c[1];
    }
}
