#include "common.h"
#include "pad.h"
#include "protos/field.h"
#include "field/n22.h"

extern u32 fldPointLightDbgSel;
extern char fldPointLightDbgNoFmt[];
extern char fldPointLightDbgDispFmt[];
extern char fldPointLightDbgIdFmt[];
extern char fldPointLightDbgRangeFmt[];
extern char fldPointLightDbgColorRFmt[];
extern char fldPointLightDbgColorGFmt[];
extern char fldPointLightDbgColorBFmt[];
extern char fldPointLightDbgPosXFmt[];
extern char fldPointLightDbgPosYFmt[];
extern char fldPointLightDbgPosZFmt[];

void FldPointLightDebugPage(void) {
    if ((padPort0[1].repeat & 0x8000) && fldPointLightDbgSel != 0) {
        fldPointLightDbgSel--;
    }
    if ((padPort0[1].repeat & 0x2000) && fldPointLightDbgSel < fldPointLightCount) {
        fldPointLightDbgSel++;
    }
    DbgPrintf(4, 4, fldPointLightDbgNoFmt, fldPointLightDbgSel, fldPointLightCount);
    DbgPrintf(4, 5, fldPointLightDbgDispFmt, fldPointLights[fldPointLightDbgSel].enable);
    DbgPrintf(4, 6, fldPointLightDbgIdFmt, fldPointLights[fldPointLightDbgSel].id);
    DbgPrintf(4, 7, fldPointLightDbgRangeFmt, fldPointLights[fldPointLightDbgSel].range);
    DbgPrintf(4, 8, fldPointLightDbgColorRFmt, fldPointLights[fldPointLightDbgSel].light.color[0]);
    DbgPrintf(4, 9, fldPointLightDbgColorGFmt, fldPointLights[fldPointLightDbgSel].light.color[1]);
    DbgPrintf(4, 10, fldPointLightDbgColorBFmt, fldPointLights[fldPointLightDbgSel].light.color[2]);
    DbgPrintf(4, 11, fldPointLightDbgPosXFmt, fldPointLights[fldPointLightDbgSel].light.pos[0]);
    DbgPrintf(4, 12, fldPointLightDbgPosYFmt, fldPointLights[fldPointLightDbgSel].light.pos[1]);
    DbgPrintf(4, 13, fldPointLightDbgPosZFmt, fldPointLights[fldPointLightDbgSel].light.pos[2]);
}
