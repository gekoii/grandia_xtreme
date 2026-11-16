/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "battle/n29.h"
#include "party.h"

typedef struct {
    u32 c0;
    u32 c1;
} ColPair;

extern ColPair btlUnitInfoCharColors[];
extern u8 btlStrFallen[];
extern u8 btlStrCommand[];
extern u8 btlStrDefense[];
extern u8 btlStrWaiting[];
extern u8 btlStrHiddenAction[];
f32 VuSinAngle(s32);

void BtlUnitInfoUpdate(void) {
    BtlUnitInfo *p = &btlUnitInfo;
    BtlUnit *ent;
    BtlUnit *e2;
    MenuFrame *o;
    PtyEnemyParams *m;
    TxtObj *b;
    s32 i;
    s8 n;
    s32 c;
    u32 t;
    s32 st;
    s64 st2;
    f32 f;

    ent = btlUnitInfo.unit;
    if (ent == NULL) {
        return;
    }
    if ((p->state & 0xF0) == 0x10) {
        if (p->skillCount > 0 && (padPort0Trigger & 8)) {
            p->detail ^= 1;
            SndSePlay3D(0x7D5, 0, 0, 0);
        }
    }
    o = &p->frame;
    switch (p->state) {
    case 0:
        o->alpha = 0.0f;
        if (ent->id < 4) {
            t = btlUnitInfoCharColors[ent->params->id - 0x1001].c0;
            o->bg.col[2].w = t;
            o->bg.col[0].w = t;
            c = btlUnitInfoCharColors[ent->params->id - 0x1001].c1;
            o->bg.col[3].w = c;
            o->bg.col[1].w = c;
        } else {
            o->bg.col[2].w = 0x93E334C;
            o->bg.col[0].w = 0x93E334C;
            o->bg.col[3].w = 0x7F574C;
            o->bg.col[1].w = 0x7F574C;
        }
        p->time = btlFrameCount;
        p->state++;
        break;
    case 1:
        if (p->time == btlFrameCount) {
            break;
        }
        TxtGlyphBufReset(&btlUnitInfoGlyphBuf);
        TxtObjSetString(&btlUnitInfoNameText, &btlUnitInfoGlyphBuf, (u8 *)ent->params->name);
        st = ent->state;
        st2 = st;
        if (ent->id < 4) {
            if (ent->params->hp <= 0) {
                TxtObjSetString(&btlUnitInfoActionText, &btlUnitInfoGlyphBuf, btlStrFallen);
            } else if (st == 0x81) {
                TxtObjSetString(&btlUnitInfoActionText, &btlUnitInfoGlyphBuf, btlStrCommand);
            } else if (st2 == 0x84) {
                TxtObjSetString(&btlUnitInfoActionText, &btlUnitInfoGlyphBuf, btlStrDefense);
            } else if (ent->action == 0x7F) {
                TxtObjSetString(&btlUnitInfoActionText, &btlUnitInfoGlyphBuf, btlStrWaiting);
            } else if (ent->action == 2) {
                TxtObjSetString(&btlUnitInfoActionText, &btlUnitInfoGlyphBuf, (u8 *)ptyItemTable[ent->skillId].name);
            } else if (ent->action == 6 && ent->comboLeader >= 0) {
                TxtObjSetString(&btlUnitInfoActionText, &btlUnitInfoGlyphBuf, (u8 *)ptyMoveTable[(btlUnits + ent->comboLeader)->skillId].name);
            } else {
                TxtObjSetString(&btlUnitInfoActionText, &btlUnitInfoGlyphBuf, (u8 *)ptyMoveTable[ent->skillId].name);
            }
        } else if (ent->action == 0x7F) {
            TxtObjSetString(&btlUnitInfoActionText, &btlUnitInfoGlyphBuf, btlStrWaiting);
        } else if (p->hidden != 0) {
            TxtObjSetString(&btlUnitInfoActionText, &btlUnitInfoGlyphBuf, btlStrHiddenAction);
        } else if ((u32)(ent->action - 6) < 2 && ent->comboLeader >= 0) {
            e2 = &btlUnits[ent->comboLeader];
            TxtObjSetString(&btlUnitInfoActionText, &btlUnitInfoGlyphBuf, (u8 *)BTL_UNIT_ACT(e2)->name);
        } else {
            TxtObjSetString(&btlUnitInfoActionText, &btlUnitInfoGlyphBuf, (u8 *)BTL_UNIT_ACT(ent)->name);
        }
        if (ent->id >= 4) {
            m = &ptyBattleParams[ent->id - 4];
            b = btlUnitInfoSkillTexts;
            n = 0;
            TxtGlyphBufReset(&btlUnitInfoSkillGlyphBuf);
            for (i = 0; i < 6; i++) {
                if (m->skillLevels[i] > 0) {
                    TxtObjSetString(b, &btlUnitInfoSkillGlyphBuf, (u8 *)ptySkillTable[m->skills[i]].name);
                    b++;
                    n++;
                }
            }
            p->skillCount = n;
            for (; n < 6; n++) {
                TxtObjSetString(b, &btlUnitInfoSkillGlyphBuf, NULL);
                b++;
            }
        } else {
            p->skillCount = 0;
        }
        p->detail = 0;
        p->state = 0x10;
        break;
    case 0x10:
        p->state++;
        o->x = 24.0f;
        p->timer = 0;
    case 0x11:
        p->timer++;
        if (p->timer >= 0x11) {
            o->y = 304.0f;
            o->alpha = 1.0f;
            p->state++;
            p->slideX = 0.0f;
            break;
        }
        f = VuSinAngle(p->timer << 10);
        o->y = 304.0f - (1.0f - f) * 64.0f;
        o->alpha = p->timer * 0.0625f;
        p->slideX = -((16 - p->timer) * 8.0f);
        break;
    case 0x20:
        p->state++;
        p->time = btlFrameCount;
        break;
    case 0x21:
        if (p->time != btlFrameCount) {
            btlUnitInfo.unit = NULL;
        }
        break;
    }
}
