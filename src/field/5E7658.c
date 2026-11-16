#include "common.h"
#include "protos/field.h"
#include "gph.h"


void MapCollModelLoad(s32 arg0, s32 arg1) {
    GphModel *o;

    FileChunkRelocate((s32 *)arg0);
    o = ChrModelCreate(arg0 + 0x10);
    o->flags |= 0x80;
    GphModelCalc(o);
    MapCollModelSet((s32)o, arg1);
}
