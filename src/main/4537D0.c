#include "common.h"
#include "main/n07.h"
#include "protos/main.h"

FilePacMotionPack *FilePacMotionRelocate(FilePacMotionPack *arg0) {
    u32 i;
    FilePacMotionEntry *e;

    if (arg0 == NULL) {
        return NULL;
    }
    e = (FilePacMotionEntry *)((s32)arg0->entries + (s32)arg0);
    arg0->entries = e;
    arg0->data = arg0->data + (s32)arg0;
    for (i = 0; i < arg0->count; i++) {
        e[i].offset += arg0->data;
    }
    for (i = 0; i < arg0->count; i++) {
        FileChunkRelocate(e[i].offset);
    }
    return arg0;
}
