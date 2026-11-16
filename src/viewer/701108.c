#include "common.h"
#include "sys.h"
#include "viewer/n35.h"
#include "protos/viewer.h"

void VuDirFromAngles(f32 *, VuVec4 *);

void VwrSceneSetup(void) {
    f32 v[4];
    VuVec4 r;
    GphLight *l;

    vwrCamera.clipFar = sysIni.clipFar;
    vwrCamera.clipNear = sysIni.clipNear;
    vwrCamera.fov = sysIni.viewAngle;
    vwrCamera.fogColor[0] = sysIni.fogColor[0];
    vwrCamera.fogColor[1] = sysIni.fogColor[1];
    vwrCamera.fogColor[2] = sysIni.fogColor[2];
    vwrCamera.fogDist = sysIni.fogDist;
    l = GphLightGet(0);
    l->color[0] = sysIni.ambient[0];
    l->color[1] = sysIni.ambient[1];
    l->color[2] = sysIni.ambient[2];
    l->color[3] = sysIni.ambient[3];
    l = GphLightGet(1);
    l->color[0] = sysIni.light0Color[0];
    l->color[1] = sysIni.light0Color[1];
    l->color[2] = sysIni.light0Color[2];
    l->color[3] = sysIni.light0Color[3];
    r.x = sysIni.light0Angle[0];
    r.y = sysIni.light0Angle[1];
    VuDirFromAngles(v, &r);
    l->dir[0] = v[0];
    l->dir[1] = v[1];
    l->dir[2] = v[2];
    l->dir[3] = 0.0f;
    l = GphLightGet(2);
    l->color[0] = sysIni.light1Color[0];
    l->color[1] = sysIni.light1Color[1];
    l->color[2] = sysIni.light1Color[2];
    l->color[3] = sysIni.light1Color[3];
    r.x = sysIni.light1Angle[0];
    r.y = sysIni.light1Angle[1];
    VuDirFromAngles(v, &r);
    l->dir[0] = v[0];
    l->dir[1] = v[1];
    l->dir[2] = v[2];
    l->dir[3] = 0.0f;
    dbgMotObjParams[0].flag = sysIni.viewerCrs;
    dbgMotObjParams[1].flag = sysIni.viewerSdw;
    dbgMotObjParams[2].flag = sysIni.viewerStg;
    SysSetClearColor(0x80000000 | (sysIni.backColor[2] << 16) | (sysIni.backColor[1] << 8) | sysIni.backColor[0]);
}
