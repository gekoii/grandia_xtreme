/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "battle/camera.h"
#include "battle/misc.h"
#include "party.h"
#include "gph.h"
#include "pad.h"
#include "sys.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void BtlChrAttachToPart(GphModel *, s32, s32);

extern char btlVictorySeFile[];
extern void (*btlSysDrawHook)(void);

#define SQ(x) ((x) * (x))

void BtlUnitStateVictory(BtlUnit *u) {
    BtlVictoryWork *w;
    BtlUnit *p;
    VuVec3 v;
    s32 i;
    s32 a;
    s32 n;
    f32 dx, dz, r, f;

    w = &btlVictoryWork;
    switch (u->phase) {
    case 0:
        switch (u->subPhase) {
        case 0:
            if ((u8)(u->motionNo - 9) < 2 && u->frame != u->loopStart) {
                if (BtlChrMotionGetEvent(u, 5, 0) == 0) {
                    return;
                }
            }
            if (btlPartyHurtLevel == 0) {
                btlBgmHandle.w = SndBgmPlay(btlHandoff.unk66, btlHandoff.unk68);
            } else {
                btlBgmHandle.w = SndBgmPlay(btlHandoff.unk6A, btlHandoff.unk6C);
            }
            BtlChrSetMotion(u, 0);
            u->alpha = 1.0f;
            u->rot[1] = (btlVictoryCamYaw + 0x6000) & 0xFFFF;
            FxTraceStopAll(u->id);
            w->frame = 0;
            w->state = 1;
            u->subPhase++;
            return;
        case 1:
            BtlVictoryMotionLoad(u);
            if (w->state == 0) {
                u->subPhase = 0;
                u->phase++;
            }
            return;
        }
        return;
    case 1:
        switch (u->subPhase) {
        case 0:
            SndStreamPlay((u->params->id - 0x1001) * 60 + (((rand() >> 15) & 2) + (btlPartyHurtLevel * 4 + 0x1070)), 0, 0);
            BtlChrSetMotionData(u, btlVictoryWork.motionData, w->motionKeys);
            u->evtLoop = 0;
            u->loopStart = u->loopEnd;
            u->headLook |= 2;
            n = BtlChrMotionFindKey(u, 1, 0);
            w->motionEnd = n;
            if (n == 0) {
                w->motionEnd = u->motionLast;
            }
            BtlCamSetMode(0, 0x10, u, BtlCamVictoryCb);
            w->state = 1;
            u->stateCount = 0;
            u->subPhase++;
        case 1:
            SndSeBankLoadStart(btlVictorySeFile, 0, 0x1A8F, 0);
            u->subPhase++;
            break;
        case 2:
            if (SndSeBankLoadGetState() == -1) {
                u->subPhase++;
            }
            break;
        case 3:
            if ((w->frame > 0x30 && (padPort0Trigger & 0x20)) || (f32)w->motionEnd <= u->frame) {
                BtlResultStart(u->id);
                u->subPhase = 0;
                u->phase++;
            }
            break;
        }
        break;
    case 2:
        break;
    }
    if (u->phase > 0) {
        if (u->stateCount == w->motionEnd) {
            if (u->params->id == 0x1003 || u->params->id == 0x1007) {
                u->headLook |= 4;
            }
        }
        if (u->params->id == 0x1006 && BtlChrMotionGetEvent(u, 3, 0) != 0) {
            BtlCamQuakeStart(5.0f);
            if (w->prop != NULL) {
                w->prop->flags &= ~8;
                BtlChrModelInvalidate(u->id);
                BtlChrAttachToPart(w->prop, BtlChrFindPart(u->model, 0x302), 0);
                ChrPartDispSetOne(u, 0x302, 0);
                btlSysDrawHook = BtlVictoryPropDraw;
            }
        }
        p = btlUnits;
        BtlChrGetPartPos(&v, u, 1);
        dx = btlCamParam.eye[0] - v.x;
        dz = btlCamParam.eye[2] - v.z;
        f = SQ(dx) + SQ(dz);
        r = (fsqrt(f) - 200.0f) * (fsqrt(f) - 200.0f);
        a = BtlAtan2Angle(dx, dz) - 0x2000;
        for (i = 0; i < 4; i++, p++) {
            if (p->state >= 0x40 && i != u->id) {
                f32 ex = p->pos[0] - v.x;
                f32 ez = p->pos[2] - v.z;
                if (r < SQ(ex) + SQ(ez) && ((BtlAtan2Angle(ex, ez) - a) & 0xFFFF) < 0x4000) {
                    p->alpha -= 0.0625f;
                    if (p->alpha < 0.0f) {
                        p->alpha = 0.0f;
                    }
                } else {
                    p->alpha += 0.0625f;
                    if (p->alpha > 1.0f) {
                        p->alpha = 1.0f;
                    }
                }
            }
        }
    }
    w->frame++;
}
