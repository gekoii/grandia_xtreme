/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "protos/viewer.h"
#include "pad.h"
#include "sys.h"
#include "ptcl.h"
#include "viewer/n35.h"

void CamDebugPadControl(void *, Pad *);

void VwrUpdate(s32 arg0) {
    s32 run;
    u16 saved;
    u32 i;
    f32 speed;
    GphModel *o;
    GphMotion *a;
    GphMotion *b;
    PtclStarterEntry *c;

    run = 0;
    if (sysState.pauseMode == 0 || (padPort1.repeat & 0x80)) {
        run = 1;
    }
    VwrViewSetup(arg0);
    if (sysState.dbgMenuFlags & 0x20000000) {
        CamDebugPadControl(&vwrCamera, &padPort1);
    }
    GphAnimStep(&vwrAnim, 1.0f);
    saved = 0;
    CamUpdateShake(&vwrCamera);
    for (i = 0; i < vwrModelList.count; i++) {
        if (dbgMotObjParams[i].mode == 0 || run == 0) {
            speed = 0.0f;
        } else {
            speed = dbgMotObjParams[i].speed;
        }
        o = vwrModelList.items[i];
        if (dbgMotObjParams[i].motionNo >= 0) {
            a = (GphMotion *)chrMotionList.items[dbgMotObjParams[i].motionNo];
            if (o->motion[0].motion != a) {
                VwrMotRequest(o, a);
            }
        } else {
            ChrModelSetMotion(o, NULL);
            o->motion[0].frame = 0.0f;
        }
        if (o->motion[0].motion != NULL && o->partCount != o->motion[0].motion->partCount) {
            ChrModelSetMotion(o, NULL);
        }
        if (dbgMotObjParams[i].mode == 2 && o->motion[0].motion != NULL) {
            saved = o->motion[0].motion->playFlags;
            o->motion[0].motion->playFlags = 0;
        }
        VwrMotBlendStep(o);
        ChrModelUpdate(o, speed);
        if (dbgMotObjParams[i].mode == 2 && o->motion[0].motion != NULL) {
            o->motion[0].motion->playFlags = saved;
            if (o->motion[0].motion->frameCount == (s32)(o->motion[0].frame + 1.0f)) {
                dbgMotObjParams[i].motionNo++;
                if ((u32)dbgMotObjParams[i].motionNo >= chrMotionList.count) {
                    dbgMotObjParams[i].motionNo = 0;
                }
            }
        }
    }
    c = PtclStarterUpdate();
    if (c != NULL && (o = vwrModelList.items[0]) != NULL) {
        o->pos[0] = c->pos[0];
        o->rot[0] = c->rot[0];
        o->pos[1] = c->pos[1];
        o->rot[1] = c->rot[1];
        o->pos[2] = c->pos[2];
        o->rot[2] = c->rot[2];
        dbgMotObjParams[0].motionNo = 0;
        b = (GphMotion *)chrMotionList.items[0];
        if (sysIni.viewerNoLoop == 0) {
            b->playFlags = 1;
        } else {
            b->playFlags = 0;
        }
        o->motion[0].frame = 0.0f;
    }
    if (run == 1) {
        PtclUpdate();
    }
}
