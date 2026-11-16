/* rodata: 0x00670C38-0x00670C60 */
#include "common.h"
#include "protos/field.h"
#include "sys.h"
#include "gph.h"
#include "field/state.h"

void MapModelLoadPac(void *);
s32 FldMemAlloc(s32);
void VuDirFromAngles(f32 *, f32 *);
void GphEnvMapSetColor(s32, f32 *);

void FldSceneSetup(s32 flagNo) {
    SysIni *m;
    GphLight *l;
    f32 dir[4];
    f32 ang[2];
    f32 rect[4];
    s32 data;
    char *name;
    void *pk;
    GphModel *model;

    m = &sysIni;
    camField[0].clipFar = m->clipFar;
    camField[0].clipNear = 1.0f;
    camField[0].fov = m->viewAngle;
    camField[0].fogColor[0] = m->fogColor[0];
    camField[0].fogColor[1] = m->fogColor[1];
    camField[0].fogColor[2] = m->fogColor[2];
    camField[0].fogDist = m->fogDist;
    l = GphLightGet(0);
    l->color[0] = m->ambient[0];
    l->color[1] = m->ambient[1];
    l->color[2] = m->ambient[2];
    l->color[3] = m->ambient[3];
    l = GphLightGet(1);
    l->color[0] = m->light0Color[0];
    l->color[1] = m->light0Color[1];
    l->color[2] = m->light0Color[2];
    l->color[3] = m->light0Color[3];
    ang[0] = m->light0Angle[0];
    ang[1] = m->light0Angle[1];
    VuDirFromAngles(dir, ang);
    l->dir[0] = dir[0];
    l->dir[1] = dir[1];
    l->dir[2] = dir[2];
    l->dir[3] = 0.0f;
    l = GphLightGet(2);
    l->color[0] = m->light1Color[0];
    l->color[1] = m->light1Color[1];
    l->color[2] = m->light1Color[2];
    l->color[3] = m->light1Color[3];
    ang[0] = m->light1Angle[0];
    ang[1] = m->light1Angle[1];
    VuDirFromAngles(dir, ang);
    l->dir[0] = dir[0];
    l->dir[1] = dir[1];
    l->dir[2] = dir[2];
    l->dir[3] = 0.0f;
    MapCollSetFlags(m->collision0);
    fldState.motionSmooth = m->motionSmooth;
    camCtl.prevType = m->cameraAi0;
    camCtl.aiType = m->cameraAi0;
    if (m->encountOn != 0) {
        EvtSystemFlagSet(100, 0);
    } else {
        EvtSystemFlagSet(100, 1);
    }
    if (sysIni.useMdtData == 0) {
        EffNop(0x10);
        name = "FIELD/TEXENV.TXD";
        if (FileGetSizeAligned(name) != 0) {
            data = FldMemAlloc(FileGetSizeAligned(name));
            FileRead(name, data);
            GphTexDataRelocate(data);
            GphEnvMapSetTex(0, data);
            rect[3] = 32.0f;
            rect[0] = 128.0f;
            rect[1] = 128.0f;
            rect[2] = 128.0f;
            GphEnvMapSetColor(0, rect);
        }
        ChrPartyLoadMember(0, 1);
        camCtl.target = ChrGetByNo(1);
        if (AmapIsLoaded() == 0) {
            pk = FldLoadModelPack("MAP.PAC");
            if (pk != NULL) {
                MapModelLoadPac(pk);
            }
            pk = FldLoadModelPack("SKY.PAC");
            if (pk != NULL) {
                FldSkyLoad(pk, 0, 0);
            }
            model = ChrGetByNo(1)->model;
            model->pos[0] = 0.0f;
            model->pos[1] = 0.0f;
            model->pos[2] = 0.0f;
            MapLoadStateIdle();
            GphFadeStart(fldFades, 0, 0.0f, 0.0f, 0.0f, 0.0f, 30.0f);
        }
    } else {
        SysSetClearColor(0x80000000);
    }
    MapRippleLoad();
}
