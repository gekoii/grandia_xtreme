/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "gph.h"
#include "field/state.h"
#include "field/n20.h"

void VuBezier3(FVECTOR *, FVECTOR *, f32);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void EvtCharSelUpdate(void) {
    FVECTOR k[4];
    u32 i;
    ChrObj *o;
    EvtCharSelWork *w;
    GphPart *e;
    s32 off;
    s32 off2;

    if (evtCharSelWork->state == 0) {
        return;
    }
    switch (evtCharSelWork->state) {
    case 1:
        o = evtCharSelWork->chars[evtCharSelWork->cur];
        copyq(&evtCharSelWork->lookFrom, &evtCharSelWork->lookKeys[0]);
        copyq(&evtCharSelWork->lookTo, &evtCharSelWork->lookKeys[0]);
        copyq(&evtCharSelWork->look, &evtCharSelWork->lookKeys[0]);
        copyq(&evtCharSelWork->cursorPos, &o->model->pos);
        copyq(&evtCharSelWork->cursorTo, &o->model->pos);
        copyq(&evtCharSelWork->cursorFrom, &o->model->pos);
        evtCharSelWork->eyeFrom[0] = 262.0f;
        evtCharSelWork->eyeFrom[1] = 443.0f;
        evtCharSelWork->eyeFrom[2] = -420.0f;
        evtCharSelWork->eyeFrom[3] = 0.0f;
        copyq(&evtCharSelWork->eyeTo, &evtCharSelWork->eyeFrom);
        copyq(&evtCharSelWork->eye, &evtCharSelWork->eyeFrom);
        evtCharSelWork->spin = 0;
        evtCharSelWork->unk1C = 0;
        if (EvtScnrIsRunning() == 0) {
            evtCharSelWork->state++;
            copyq(&evtCharSelWork->eyeTo, &evtCharSelWork->eyeKey);
            evtCharSelWork->eyeT = 0.0f;
            if (evtCharSelWork->cursor != NULL) {
                evtCharSelWork->cursor->scale[0] = 0.0f;
                evtCharSelWork->cursor->flags = 0;
                evtCharSelWork->cursor->scale[2] = evtCharSelWork->cursor->scale[1] = evtCharSelWork->cursor->scale[0];
            }
            if (evtCharSelWork->cursor2 != NULL) {
                evtCharSelWork->cursor2->scale[0] = 0.0f;
                evtCharSelWork->cursor2->flags = 0;
                evtCharSelWork->cursor2->scale[2] = evtCharSelWork->cursor2->scale[1] = evtCharSelWork->cursor2->scale[0];
                e = ChrModelFindPart(evtCharSelWork->cursor2, 1);
                if (e != NULL) {
                    e->flags = 0;
                }
            }
            evtCharSelWork->spinSpeed = 768.0f;
        }
        break;
    case 2:
        if (padPort0[0].trigger & 0x8000) {
            if (evtCharSelWork->cur != 0) {
                evtCharSelWork->spinSpeed = -2000.0f;
                evtCharSelWork->cur--;
            } else {
                evtCharSelWork->cur = evtCharSelWork->count - 1;
                evtCharSelWork->spinSpeed = 2000.0f;
            }
            copyq(&evtCharSelWork->lookFrom, &evtCharSelWork->look);
            off = evtCharSelWork->cur * 16 + 0x130; /* &lookKeys[cur] as a byte offset */
            copyq(&evtCharSelWork->lookTo, (u8 *)evtCharSelWork + off);
            o = evtCharSelWork->chars[evtCharSelWork->cur];
            copyq(&evtCharSelWork->cursorFrom, &evtCharSelWork->cursorPos);
            copyq(&evtCharSelWork->cursorTo, &o->model->pos);
            evtCharSelWork->lookT = 0.0f;
            evtCharSelWork->cursorT = 0.0f;
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        }
        if (padPort0[0].trigger & 0x2000) {
            if (evtCharSelWork->cur < evtCharSelWork->count - 1) {
                evtCharSelWork->cur++;
                evtCharSelWork->spinSpeed = 2000.0f;
            } else {
                evtCharSelWork->cur = 0;
                evtCharSelWork->spinSpeed = -2000.0f;
            }
            copyq(&evtCharSelWork->lookFrom, &evtCharSelWork->look);
            off2 = evtCharSelWork->cur * 16 + 0x130;
            copyq(&evtCharSelWork->lookTo, (u8 *)evtCharSelWork + off2);
            o = evtCharSelWork->chars[evtCharSelWork->cur];
            copyq(&evtCharSelWork->cursorFrom, &evtCharSelWork->cursorPos);
            copyq(&evtCharSelWork->cursorTo, &o->model->pos);
            evtCharSelWork->lookT = 0.0f;
            evtCharSelWork->cursorT = 0.0f;
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
        }
        if (padPort0[0].trigger & 0x20) {
            o = evtCharSelWork->chars[evtCharSelWork->cur];
            EvtScnrCall((u16)o->id);
            evtCharSelWork->state = 3;
        }
        break;
    case 3:
        if (EvtScnrIsRunning() == 0) {
            evtCharSelWork->state = 2;
        }
        break;
    case 4:
        evtCharSelWork->state = 0;
        break;
    }

    for (i = 0; i < evtCharSelWork->count; i++) {
        if (ChrMotIsEnd(evtCharSelWork->chars[i]->model) == 1) {
            if (evtCharSelWork->idleCount[i] != 0) {
                evtCharSelWork->chars[i]->mot = 3;
                evtCharSelWork->idleCount[i]--;
            } else {
                evtCharSelWork->idleCount[i] = VuRandInt(5) + 2;
                if ((s32)VuRandInt(100) < 50) {
                    evtCharSelWork->chars[i]->mot = 4;
                } else {
                    evtCharSelWork->chars[i]->mot = 5;
                }
            }
            ChrMotSetImmediate(evtCharSelWork->chars[i], evtCharSelWork->chars[i]->mot);
            ChrMotFlagsSet(evtCharSelWork->chars[i]->model, 2, 0);
            ChrMotFlagsSet(evtCharSelWork->chars[i]->model, 4, 1);
            ChrMotSetFrame(evtCharSelWork->chars[i]->model, 0.0f);
        }
    }

    copyq(&k[0], &evtCharSelWork->lookFrom);
    copyq(&k[1], &evtCharSelWork->lookTo);
    copyq(&k[2], &evtCharSelWork->lookTo);
    copyq(&k[3], &evtCharSelWork->lookTo);
    VuBezier3(&evtCharSelWork->look, k, evtCharSelWork->lookT);
    evtCharSelWork->lookT += 0.03f;
    if (evtCharSelWork->lookT >= 1.0f) {
        evtCharSelWork->lookT = 1.0f;
    }

    if (evtCharSelWork->cursor != NULL) {
        evtCharSelWork->spinSpeed *= 0.98f;
        if (evtCharSelWork->spinSpeed > 0.0f) {
            if (evtCharSelWork->spinSpeed < 800.0f) {
                evtCharSelWork->spinSpeed = 800.0f;
            }
        } else {
            if (evtCharSelWork->spinSpeed > -800.0f) {
                evtCharSelWork->spinSpeed = -800.0f;
            }
        }
        w = evtCharSelWork; /* dead load: makes the next block reuse this %hi */
        if (EvtSystemFlagGet(0x60) == 1 && EvtSystemFlagGet(evtCharSelWork->flagIds[evtCharSelWork->cur]) == 1) {
            if (evtCharSelWork->cursor->scale[0] > 0.0f) {
                evtCharSelWork->cursor->scale[0] -= 0.05f;
            }
            if (evtCharSelWork->cursor->scale[0] < 0.0f) {
                evtCharSelWork->cursor->scale[0] = 0.0f;
            }
            if (evtCharSelWork->cursor->scale[0] < 0.1f) {
                if (evtCharSelWork->cursor2->scale[0] < 1.0f) {
                    evtCharSelWork->cursor2->scale[0] += 0.05f;
                }
                if (evtCharSelWork->cursor2->scale[0] >= 1.0f) {
                    evtCharSelWork->cursor2->scale[0] = 1.0f;
                }
            }
            if (evtCharSelWork->cursor2->scale[0] == 1.0f) {
                e = ChrModelFindPart(evtCharSelWork->cursor2, 1);
                if (e != NULL) {
                    if (e->rot[1] < 0x5000) {
                        e->rot[1] += 0x400;
                    }
                    if (e->rot[1] > 0x5000) {
                        e->rot[1] = 0x5000;
                    }
                }
            }
        } else {
            evtCharSelWork->spin += (s32)evtCharSelWork->spinSpeed;
            if (evtCharSelWork->cursor2->scale[0] < 0.1f) {
                if (evtCharSelWork->cursor->scale[0] < 1.0f) {
                    evtCharSelWork->cursor->scale[0] += 0.05f;
                }
                if (evtCharSelWork->cursor->scale[0] > 1.0f) {
                    evtCharSelWork->cursor->scale[0] = 1.0f;
                }
            }
            if (evtCharSelWork->cursor2->scale[0] > 0.0f) {
                evtCharSelWork->cursor2->scale[0] -= 0.05f;
            }
            if (evtCharSelWork->cursor2->scale[0] < 0.0f) {
                evtCharSelWork->cursor2->scale[0] = 0.0f;
                e = ChrModelFindPart(evtCharSelWork->cursor2, 1);
                if (e != NULL) {
                    e->rot[1] = 0;
                }
            }
        }
        evtCharSelWork->cursor->scale[2] = evtCharSelWork->cursor->scale[1] = evtCharSelWork->cursor->scale[0];
        evtCharSelWork->cursor2->scale[2] = evtCharSelWork->cursor2->scale[1] = evtCharSelWork->cursor2->scale[0];
        evtCharSelWork->cursor->rot[1] = FldCalcYaw(&evtCharSelWork->cursor->pos, camField[0].eye) + evtCharSelWork->spin;
        evtCharSelWork->cursor2->rot[1] = FldCalcYaw(&evtCharSelWork->cursor->pos, camField[0].eye);
        evtCharSelWork->bobFrom[0] = 10.0f;
        evtCharSelWork->bobTo[0] = 20.0f;
        evtCharSelWork->bobTo[1] = 1.2f;
        evtCharSelWork->bob[0] = 0.0f;
        evtCharSelWork->bob[1] = 1.0f;
        evtCharSelWork->bob[2] = 0.0f;
        evtCharSelWork->bob[3] = 0.0f;
        evtCharSelWork->bobFrom[1] = 1.0f;
        evtCharSelWork->bobFrom[2] = 0.0f;
        evtCharSelWork->bobFrom[3] = 0.0f;
        evtCharSelWork->bobTo[2] = 0.0f;
        evtCharSelWork->bobTo[3] = 0.0f;
        copyq(&k[0], &evtCharSelWork->bobFrom);
        copyq(&k[1], &evtCharSelWork->bobTo);
        copyq(&k[2], &evtCharSelWork->bobTo);
        copyq(&k[3], &evtCharSelWork->bobFrom);
        VuBezier3(&evtCharSelWork->bob, k, evtCharSelWork->bobT);
        evtCharSelWork->bobT += 0.01f;
        if (evtCharSelWork->bobT >= 1.0f) {
            evtCharSelWork->bobT -= 1.0f;
        }
        copyq(&k[0], &evtCharSelWork->cursorFrom);
        copyq(&k[1], &evtCharSelWork->cursorTo);
        copyq(&k[2], &evtCharSelWork->cursorTo);
        copyq(&k[3], &evtCharSelWork->cursorTo);
        VuBezier3(&evtCharSelWork->cursorPos, k, evtCharSelWork->cursorT);
        copyq(&evtCharSelWork->cursor->pos, &evtCharSelWork->cursorPos);
        copyq(&evtCharSelWork->cursor2->pos, &evtCharSelWork->cursor->pos);
        evtCharSelWork->cursor->pos[1] += evtCharSelWork->bob[0];
        evtCharSelWork->cursor2->pos[1] += evtCharSelWork->bob[0];
        evtCharSelWork->cursor->pos[1] += 200.0f;
        evtCharSelWork->cursor2->pos[1] += 170.0f;
        evtCharSelWork->cursorT += 0.04f;
        if (evtCharSelWork->cursorT >= 1.0f) {
            evtCharSelWork->cursorT = 1.0f;
        }
    }

    copyq(&k[0], &evtCharSelWork->eyeFrom);
    copyq(&k[1], &evtCharSelWork->eyeTo);
    copyq(&k[2], &evtCharSelWork->eyeTo);
    copyq(&k[3], &evtCharSelWork->eyeTo);
    VuBezier3(&evtCharSelWork->eye, k, evtCharSelWork->eyeT);
    evtCharSelWork->eyeT += 0.01f;
    if (evtCharSelWork->eyeT >= 1.0f) {
        evtCharSelWork->eyeT = 1.0f;
    }
    EvtCharSelCamSet(camField);
}
