#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/camera.h"
#include "gph.h"

void VuMtxStackPush(u32);
void VuMtxStackLoad(void *);
f32 VuSinAngle(s32);

void BtlCmdMenuBuild(void) {
    BtlCmdMenu *m = &btlCmdMenu;
    BtlCmdRingCam *c;
    BtlCmdAreaReq *r;
    GphPart *o;
    GphPart *o2;
    GphModel *o3;
    GphPart *oi;
    s32 i;
    s32 j;
    s32 t;
    f32 f;

    if (m->state == 0) {
        return;
    }
    if (m->state == 0x10) {
        c = &m->ringCam;
        for (i = 0; i < 8; i++) {
            o = BtlChrFindPart(btlCmdMenuRingInst, i + 1);
            o->scale[0] = o->scale[1] = o->scale[2] = 0.6f;
        }
        o = BtlChrFindPart(btlCmdMenuRingInst, m->sel + 1);
        f = VuSinAngle(m->ringCam.inputTimer << 9) * 0.2f + 0.9f;
        o->scale[0] = o->scale[1] = o->scale[2] = f;
        o = BtlChrFindPart(btlCmdMenuRingInst, 9);
        o->rot[0] = c->angle;
        o = BtlChrFindPart(btlCmdMenuRingInst, 10);
        o->rot[1] = c->unk18;
        o->pos[0] = c->unk20 + c->tilt;
        o->rot[2] = c->unk1C;
        o->pos[1] = c->unk24;
        o->scale[0] = o->scale[1] = o->scale[2] = c->unk28;
        VuMtxStackPush(0);
        CamApply(&btlCmdMenuCam);
        VuMtxStackLoad(&btlCmdMenuCam);
        GphDrawListSet(btlCmdMenuRingDrawList);
        GphDrawListClear(0);
        BtlChrModelCalcDraw(btlCmdMenuRingInst);
        GphDrawListSort();
        VuMtxStackPop();
        CamApply(btlCamera);
        VuMtxStackLoad(btlCamera);
    }
    GphDrawListSet(btlCmdMenuDrawList);
    GphDrawListClear(0);
    if (m->area.active != 0) {
        r = &m->area;
        switch ((s8)(r->type - 2)) {
        case 0:
        case 4:
            btlCmdMenuCircleArea->pos[0] = (*r->center)[0];
            btlCmdMenuCircleArea->pos[2] = (*r->center)[2];
            btlCmdMenuCircleArea->scale[0] = btlCmdMenuCircleArea->scale[2] = r->scale;
            o2 = BtlChrFindPart(btlCmdMenuCircleArea, 1);
            o2->scale[0] = o2->scale[2] = (f32)r->size * 0.0002f;
            if (r->prevSize > 0) {
                o2 = BtlChrFindPart(btlCmdMenuCircleArea, 2);
                o2->scale[0] = o2->scale[2] = (f32)r->prevSize * 0.0002f;
            }
            ChrModelSetAlpha(btlCmdMenuCircleArea, (r->fade * 0.5f + 0.5f - 1.0f) * 128.0f);
            BtlChrModelCalcDraw(btlCmdMenuCircleArea);
            break;
        case 2:
            btlCmdMenuLineArea->pos[0] = m->unit->pos[0];
            btlCmdMenuLineArea->pos[2] = m->unit->pos[2];
            btlCmdMenuLineArea->rot[1] = 0x8000 - BtlAtan2Angle((*r->center)[0] - m->unit->pos[0], (*r->center)[2] - m->unit->pos[2]);
            oi = BtlChrFindPart(btlCmdMenuLineArea, 1);
            t = r->scale * r->size * 182.04445f * 0.5f;
            oi->pos[0] = 0;
            oi->rot[1] = t;
            oi = BtlChrFindPart(btlCmdMenuLineArea, 2);
            oi->pos[0] = 0;
            oi->rot[1] = -t;
            if (r->prevSize > 0) {
                t = r->scale * r->prevSize * 182.04445f * 0.5f;
                oi = BtlChrFindPart(btlCmdMenuLineArea, 3);
                oi->pos[0] = 0;
                oi->rot[1] = t;
                oi = BtlChrFindPart(btlCmdMenuLineArea, 4);
                oi->pos[0] = 0;
                oi->rot[1] = -t;
            }
            ChrModelSetAlpha(btlCmdMenuLineArea, (r->fade * 0.5f + 0.5f - 1.0f) * 128.0f);
            BtlChrModelCalcDraw(btlCmdMenuLineArea);
            break;
        case 1:
            btlCmdMenuLineArea->pos[0] = m->unit->pos[0];
            btlCmdMenuLineArea->pos[2] = m->unit->pos[2];
            btlCmdMenuLineArea->rot[1] = 0x8000 - BtlAtan2Angle((*r->center)[0] - m->unit->pos[0], (*r->center)[2] - m->unit->pos[2]);
            {
                GphPart *of1 = BtlChrFindPart(btlCmdMenuLineArea, 1);
                of1->pos[0] = r->scale * r->size;
                of1->rot[1] = 0;
            }
            {
                GphPart *of2 = BtlChrFindPart(btlCmdMenuLineArea, 2);
                of2->pos[0] = -r->scale * r->size;
                of2->rot[1] = 0;
            }
            if (r->prevSize > 0) {
                {
                    GphPart *of3 = BtlChrFindPart(btlCmdMenuLineArea, 3);
                    of3->pos[0] = r->scale * r->prevSize;
                    of3->rot[1] = 0;
                }
                {
                    GphPart *of4 = BtlChrFindPart(btlCmdMenuLineArea, 4);
                    of4->pos[0] = -r->scale * r->prevSize;
                    of4->rot[1] = 0;
                }
            }
            ChrModelSetAlpha(btlCmdMenuLineArea, (r->fade * 0.5f + 0.5f - 1.0f) * 128.0f);
            BtlChrModelCalcDraw(btlCmdMenuLineArea);
            break;
        case 3:
            btlCmdMenuAllArea->scale[0] = btlCmdMenuAllArea->scale[2] = r->scale;
            ChrModelSetAlpha(btlCmdMenuAllArea, (r->fade * 0.5f + 0.5f - 1.0f) * 128.0f);
            BtlChrModelCalcDraw(btlCmdMenuAllArea);
            break;
        }
    }
    if (m->state == 0x41) {
        for (j = 0; j < 6; j++) {
            o3 = btlCmdMenuPoints[j];
            o3->pos[0] = m->points[j].x;
            o3->pos[1] = 0.0f;
            o3->pos[2] = m->points[j].z;
            BtlChrModelCalcDraw(o3);
        }
        if (m->unkAC != 0) {
            BtlChrModelCalcDraw(btlCmdMenuAllyCursor);
        }
    } else if ((m->state & 0x40) && m->targetCount < 0x10 && m->state != 0x42) {
        if (m->unit->target < 4) {
            BtlChrModelCalcDraw(btlCmdMenuAllyCursor);
        } else {
            BtlChrModelCalcDraw(btlCmdMenuEnemyCursor);
        }
    }
    GphDrawListSort();
}
