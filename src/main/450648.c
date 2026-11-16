#include "common.h"
#include "ptcl.h"

extern s32 ptclEmitterStripIndices;
extern s32 ptclEmitterUvBlock;

void PtclEmitterSysInit(void) {
    ptclEmitterPause = 0;
    ptclEmitterDrawCtx = (GphGaObject *)0x70003F80;
    ptclEmitterDrawTemplate.mode = 0x22;
    ptclEmitterDrawTemplate.clip = 1;
    ptclEmitterDrawTemplate.center[3] = 1.0f;
    ptclEmitterDrawTemplate.triCount = 0x3E;
    ptclEmitterDrawTemplate.vtxCount = 0x7C;
    ptclEmitterDrawTemplate.indices = &ptclEmitterStripIndices;
    ptclEmitterDrawTemplate.uvs = &ptclEmitterUvBlock;
    ptclEmitterDrawTemplate.center[0] = 0.0f;
    ptclEmitterDrawTemplate.center[1] = 0.0f;
    ptclEmitterDrawTemplate.center[2] = 0.0f;
    ptclEmitterDrawTemplate.unk30 = 0.0f;
    ptclEmitterDrawTemplate.lightBlock = 0;
    ptclEmitterDrawTemplate.normals = NULL;
    ptclEmitterDrawTemplate.unk70 = 0;
    ptclEmitterDrawTemplate.unk74 = 0;
    ptclEmitterDrawTemplate.uOffset = 0.0f;
    ptclEmitterDrawTemplate.vOffset = 0.0f;
}
