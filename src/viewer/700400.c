/* rodata: 0x00703388-0x007033D0 */
#include "common.h"
#include "protos/viewer.h"
#include "sys.h"
#include "viewer/n35.h"


void VwrLoad(void) {
    s32 xyz;
    s32 stage;
    s32 size;
    s32 data;
    u32 i;
    f32 x;
    GphModel *o;

    MemHeapSelect(sysState.dataHeap);
    PtclStarterInit();
    vwrStatInstBytes = 0;
    vwrStatFileBytes = 0;
    vwrStatSortBytes = 0;
    VwrLoadModelPack("MODEL.PAC", 1);
    xyz = VwrLoadModelPack("XYZ.PAC", 0);
    stage = VwrLoadModelPack("STAGE.PAC", 0);
    VwrLoadMotionPack("MOTION.PAC");
    PtclDbgSetExtSizes(vwrStatFileBytes, vwrStatSortBytes, vwrStatInstBytes);
    size = FileGetSizeAligned("DATA/TEXENV.TXD");
    if (size != 0) {
        data = MemAlloc128(size);
        FileRead("DATA/TEXENV.TXD", data);
        GphTexDataRelocate(data);
        GphEnvMapSetTex(0, data);
    }
    x = 0.0f;
    for (i = 0; i < vwrModelList.count; i++) {
        o = vwrModelList.items[i];
        o->pos[0] = x;
        x += 250.0f;
        o->scale[2] = 1.0f;
        o->scale[1] = 1.0f;
        o->scale[0] = 1.0f;
    }
    if (stage != 0) {
        o = vwrModelList.items[vwrModelList.count - 1];
        o->pos[1] = -1.0f;
        o->pos[0] = 0.0f;
        o->scale[2] = 1.0f;
        o->scale[1] = 1.0f;
        o->scale[0] = 1.0f;
    }
    if (xyz != 0) {
        if (stage != 0) {
            o = vwrModelList.items[vwrModelList.count - 2];
        } else {
            o = vwrModelList.items[vwrModelList.count - 1];
        }
        o->pos[0] = 0.0f;
        o->scale[2] = 1.0f;
        o->scale[1] = 1.0f;
        o->scale[0] = 1.0f;
    }
    if (sysIni.viewerPtc == 0) {
        sysState.dbgMenuFlags = 0x01700000;
    } else {
        sysState.dbgMenuFlags = 0x01C10000;
    }
    GphScreenFadeStart(0, 0.0f, 0.0f, 0.0f, 0.0f, 30.0f);
}
