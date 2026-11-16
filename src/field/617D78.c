/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n22.h"
#include "field/event.h"

extern void VuVecCopy(f32 *, s32);

void FldSndUpdate(u8 *arg0, f32 dist) {
    EvtScnrSndStatus *p;
    s32 t;
    u32 i;
    FldSndEmitter *o;

    if (SysDemoGetMode()) {
        return;
    }
    SndListenerSetFromCamera(camField);
    if (EvtSystemFlagGet(0x8B) == 1) {
        if (fldSnd.strmHandle != 0xFFFFFFFF) {
            t = SndBgmGetState(fldSnd.strmHandle);
            if (t == 12 || t == 0) {
                EvtSystemFlagSet(0x8B, 0);
            }
        }
    }
    p = evtScnrSndStatus;
    if (fldSnd.bgmHandle != 0xFFFFFFFF) {
        switch (SndBgmGetState(fldSnd.bgmHandle)) {
        case 0:
            p[0].state = 0;
            p[0].id = -1;
            p[0].time = -1;
            break;
        case 12:
            p[0].state = 2;
            p[0].id = fldSnd.bgmId;
            p[0].time = FldSndBgmGetPlayPos();
            break;
        default:
            p[0].state = 1;
            p[0].id = fldSnd.bgmId;
            p[0].time = 0;
            break;
        }
    }
    if (fldSnd.strmHandle != 0xFFFFFFFF) {
        switch (SndBgmGetState(fldSnd.strmHandle)) {
        case 0:
            p[1].state = 0;
            p[1].id = -1;
            p[1].time = -1;
            break;
        case 11:
            fldSndStreamFrames = 0;
            break;
        case 12:
            p[1].state = 2;
            p[1].id = fldSnd.strmId;
            p[1].time = FldSndStreamGetPlayPos();
            fldSndStreamFrames++;
            break;
        default:
            p[1].state = 1;
            p[1].id = fldSnd.strmId;
            p[1].time = 0;
            break;
        }
    }
    if (SndSeIsActive()) {
        for (i = 0; i < fldSndEmitterCount; i++) {
            o = &fldSndEmitters[i];
            if (!(o->flags & 1)) {
                if (o->slot >= 0) {
                    SndSeStop(2, o->slot);
                    o->slot = -1;
                }
            } else {
                if (o->attach != 0) {
                    VuVecCopy(o->pos, o->attach + 0x10);
                    if (o->slot >= 0) {
                        SndSeSetPos(2, o->slot, o->pos);
                    }
                }
                switch (o->state) {
                case 0:
                    if (FldVecDist(o->pos, arg0 + 0x50) < dist) {
                        if (o->slot < 0) {
                            o->slot = SndSePlay3D(o->se, 2, 0x18, o->pos);
                        } else {
                            t = SndSeGetSlotState(2, o->slot);
                            if (t == 4 || t == 0) {
                                SndSeClearHold(2, o->slot);
                                o->slot = SndSePlay3D(o->se, 2, 8, o->pos);
                            }
                        }
                        if (o->flags & 4) {
                            o->state = 1;
                        }
                        o->timer = 0;
                    }
                    break;
                case 1:
                    if (++o->timer >= o->period + o->unk34) {
                        o->timer = 0;
                        if (o->flags & 4) {
                            o->state = 0;
                            SndSeStop(2, o->slot);
                        }
                    }
                    break;
                }
            }
        }
    }
}
