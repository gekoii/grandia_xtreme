/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "vu.h"
#include "battle/misc.h"
#include "battle/camera.h"
#include "battle/unit.h"
#include "party.h"
#include "gph.h"
#include "battle/n28.h"

extern u8 btlRageCaption[];
extern s16 btlCharIntroVoiceSe[];

/* Not in protos/battle.h (their definitions use local struct types). */
/* BtlCamQuakeSetY is (s32, f32, f32); the calls go through (f32, f32, s32):
 * EABI passes the int in a0 either way, and this order gives the original
 * argument-load order. */

void BtlSpecialIntroUpdate(void) {
    BtlSpecialIntro *p = &btlSpecialIntro;
    BtlUnit *w;
    VuVec3 v;
    s32 i;

    if (p->unit != NULL) {
        if (p->unit->params->angryProb >= 2.0f) {
            switch (p->state) {
            case 0:
                BtlCaptionShow(btlRageCaption, 0xB4, p->unit);
                BtlFadeStart(0, 0xC0000020, 0x50);
                p->timer = 0;
                p->state++;
            case 1:
                if (p->timer == 0x3C) {
                    BtlUnitFadeTo(p->unit, 7, 1, 0.0f);
                    BtlUnitFadeTo(p->unit, 1, 1, 1.0f);
                    BtlCamSetMode(0, 0, p->unit, BtlSpecialIntroCamRage);
                    ((void (*)(f32, f32, s32))BtlCamQuakeSetY)(2.0f, 2.0f, 0xA);
                    btlCamera->shakeCycle[1] = 2.0f;
                    BtlCamQuakeSetInterval(1);
                    p->state++;
                    btlCamParam.flags |= 4;
                }
                break;
            case 2:
                if (p->timer == 0x3D) {
                    if (p->unit->model != 0) {
                        GphPart *m = ChrModelFindPart(p->unit->model, 0x280);
                        if (m != NULL) {
                            m->rot[1] = 0;
                            m->rot[0] = -0x2000;
                        }
                        ChrFaceAnimSet(&p->unit->face, 2, 0);
                    }
                } else if (p->timer >= 0xCB && p->timer <= 0xD2) {
                    if (p->unit->model != 0) {
                        GphPart *m = ChrModelFindPart(p->unit->model, 0x280);
                        if (m != NULL) {
                            m->rot[0] = (p->timer - 0xD2) << 10;
                        }
                    }
                    if (p->timer == 0xD2) {
                        ChrFaceAnimSet(&p->unit->face, 3, 0);
                        BtlChrGetPartPos(&v, p->unit, 1);
                        PtclBtlDustSpawn(0x80, 0, v.x, v.z);
                        ((void (*)(f32, f32, s32))BtlCamQuakeSetY)(100.0f, 50.0f, 0);
                        ((void (*)(f32, f32, s32))BtlCamQuakeSetY)(8.0f, 5.0f, 0x1E);
                        BtlCamQuakeSetInterval(0);
                        btlCamera->shakeCycle[1] = 4.0f;
                    }
                } else if (p->timer == 0xDC) {
                    BtlCamSetMode(0, 0x18, p->unit, BtlSpecialIntroCamRagePull);
                    btlCamParam.flags |= 4;
                    BtlFadeStart(0, 0xC0000000, 0x5A);
                    BtlChrGetPartPos(&v, p->unit, 1);
                    PtclBtlDustSpawn(0x81, 0, v.x, v.z);
                } else if (p->timer == 0xE6) {
                    BtlChrGetPartPos(&v, p->unit, 1);
                    PtclBtlDustSpawn(0x82, 0, v.x, v.z);
                } else if (p->timer >= 0xF0) {
                    p->unit->params->angryProb = 3.0f;
                    BtlUnitFadeTo(p->unit, 7, 0x20, 1.0f);
                    BtlCamQuakeStop(0x5A);
                    p->timer = 0;
                    p->state++;
                }
                break;
            case 3:
                if (p->timer >= 0x1E) {
                    ChrFaceAnimSet(&p->unit->face, 0, 1);
                    w = p->unit;
                    p->unit = NULL;
                    w->flags |= 8;
                }
                break;
            }
        } else if (p->unit->comboPartners[0] >= 0) {
            switch (p->state) {
            case 0:
                SndBgmFadeVolume(btlBgmHandle.b, 1, 5.0f);
                p->count = 1;
                if (p->unit->model != 0) {
                    GphPart *m = ChrModelFindPart(p->unit->model, 0x280);
                    if (m != NULL) {
                        m->rot[1] = 0;
                        m->rot[0] = 0;
                    }
                }
                for (i = 0; i < 3 && p->unit->comboPartners[i] >= 0; i++) {
                    p->cur = &btlUnits[p->unit->comboPartners[i]];
                    BtlChrSetMotion(p->cur, 5);
                    BtlChrMotionSetFrame(p->cur, BtlChrMotionFindKey(p->cur, 0x407, 0) - 1.0f);
                    if (p->cur->model != 0) {
                        GphPart *m = ChrModelFindPart(p->cur->model, 0x280);
                        if (m != NULL) {
                            m->rot[1] = 0;
                            m->rot[0] = 0;
                        }
                    }
                    p->count++;
                }
                p->timer = 0;
                p->state = 5 - p->count;
            case 1:
            case 2:
            case 3:
            case 4:
                if (p->timer == 0) {
                    switch (p->state) {
                    case 1:
                        p->cur = &btlUnits[p->unit->comboPartners[2]];
                        BtlCamSetMode(0, 0, p->cur, BtlSpecialIntroCamMember3);
                        break;
                    case 2:
                        p->cur = &btlUnits[p->unit->comboPartners[1]];
                        BtlCamSetMode(0, 0, p->cur, BtlSpecialIntroCamMember2);
                        break;
                    case 3:
                        p->cur = &btlUnits[p->unit->comboPartners[0]];
                        BtlCamSetMode(0, 0, p->cur, BtlSpecialIntroCamMember1);
                        break;
                    default:
                        p->cur = p->unit;
                        BtlCamSetMode(0, 0, p->cur, BtlSpecialIntroCamLeader);
                        break;
                    }
                    SndSePlay3D(btlCharIntroVoiceSe[p->cur->params->id - 0x1001], 0, 0, NULL);
                    BtlUnitFadeTo(p->cur, 7, 1, 0.0f);
                    BtlUnitFadeTo(p->cur, 1, 1, 1.0f);
                    if (p->state != 5 - p->count) {
                        GphShatterStart(0xA);
                    }
                } else if (p->timer == 1) {
                    if (p->state == 5 - p->count) {
                        switch (p->count) {
                        default: {
                            BtlUnit *w0;

                            p->unit->rot[1] = 0x9000;
                            w0 = &btlUnits[p->unit->comboPartners[0]];
                            w0->rot[1] = 0x7000;
                            p->unit->pos[2] = p->unit->pos[0] = -60.0f;
                            w0->pos[2] = w0->pos[0] = 60.0f;
                            p->cur = w0;
                            break;
                        }
                        case 3: {
                            BtlUnit *w0, *w1;

                            w0 = &btlUnits[p->unit->comboPartners[0]];
                            p->unit->pos[2] = -180.0f;
                            p->unit->pos[0] = 0.0f;
                            p->unit->rot[1] = 0x8000;
                            w0->pos[0] = -140.0f;
                            w0->rot[1] = 0x9000;
                            w0->pos[2] = 50.0f;
                            p->cur = w0;
                            w1 = &btlUnits[p->unit->comboPartners[1]];
                            w1->rot[1] = 0x7000;
                            w1->pos[0] = 140.0f;
                            w1->pos[2] = 50.0f;
                            p->cur = w1;
                            break;
                        }
                        case 4: {
                            BtlUnit *w0, *w1, *w2;

                            p->unit->pos[0] = -20.0f;
                            w0 = &btlUnits[p->unit->comboPartners[0]];
                            p->unit->pos[2] = -180.0f;
                            p->unit->rot[1] = 0x8000;
                            w0->pos[0] = 200.0f;
                            w0->pos[2] = 0.0f;
                            w0->rot[1] = 0x7000;
                            p->cur = w0;
                            w1 = &btlUnits[p->unit->comboPartners[1]];
                            w1->pos[0] = -200.0f;
                            w1->pos[2] = 0.0f;
                            w1->rot[1] = 0x9000;
                            p->cur = w1;
                            w2 = &btlUnits[p->unit->comboPartners[2]];
                            w2->rot[1] = 0x8000;
                            w2->pos[0] = 20.0f;
                            w2->pos[2] = 180.0f;
                            p->cur = w2;
                            break;
                        }
                        }
                    }
                    switch (p->state) {
                    case 1:
                        BtlCamSetMode(0, 0x3C, p->cur, BtlSpecialIntroCamMember3Pose);
                        break;
                    case 2:
                        BtlCamSetMode(0, 0x3C, p->cur, BtlSpecialIntroCamMember2Pose);
                        break;
                    case 3:
                        BtlCamSetMode(0, 0x3C, p->cur, BtlSpecialIntroCamMember1Pose);
                        break;
                    default:
                        BtlCamSetMode(0, 0x64, p->cur, BtlSpecialIntroCamGroup);
                        break;
                    }
                }
                if (p->state == 4) {
                    if (p->timer == 0x32) {
                        for (i = 0; i < 3 && p->unit->comboPartners[i] >= 0; i++) {
                            BtlUnitFadeTo(&btlUnits[p->unit->comboPartners[i]], 1, 0x20, 1.0f);
                        }
                    } else if (p->timer >= 0x78) {
                        p->state = 5;
                        p->timer = -1;
                    }
                } else if (p->timer >= 0x3C) {
                    p->state++;
                    p->timer = -1;
                }
                break;
            case 5:
                SndBgmFadeVolume(btlBgmHandle.b, 0x1E, 80.0f);
                BtlUnitFadeTo(NULL, 7, 0x20, 1.0f);
                w = p->unit;
                p->unit = NULL;
                w->flags |= 8;
                break;
            }
        } else {
            switch (p->state) {
            case 0:
                SndBgmFadeVolume(btlBgmHandle.b, 1, 5.0f);
                SndSePlay3D(btlCharIntroVoiceSe[p->unit->params->id - 0x1001], 0, 0, NULL);
                BtlUnitFadeTo(p->unit, 7, 1, 0.0f);
                BtlUnitFadeTo(p->unit, 1, 1, 1.0f);
                if (p->unit->model != 0) {
                    GphPart *m = ChrModelFindPart(p->unit->model, 0x280);
                    if (m != NULL) {
                        m->rot[1] = 0;
                        m->rot[0] = 0;
                    }
                }
                GphScreenFxSetHold(&btlScreenFx, 1, 1, 4, 60.0f);
                p->timer = 0;
                p->state++;
            case 1:
            case 2:
            case 3:
                if (p->timer == 0) {
                    BtlCamSetMode(0, 0, p->unit, BtlSpecialIntroCamFace);
                } else if (p->timer == 1) {
                    BtlCamSetMode(0, 0x1E, p->unit, BtlSpecialIntroCamZoom);
                } else if (p->timer >= 0x18) {
                    p->timer = -1;
                    p->state++;
                }
                break;
            case 4:
                SndBgmFadeVolume(btlBgmHandle.b, 0x1E, 80.0f);
                BtlUnitFadeTo(p->unit, 7, 0x20, 1.0f);
                GphScreenFxSetHold(&btlScreenFx, 0, 1, 0x10, 0.0f);
                w = p->unit;
                p->unit = NULL;
                w->flags |= 8;
                break;
            }
        }
        p->timer++;
    }
}
