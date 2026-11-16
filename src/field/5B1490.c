/* rodata: 0x00670C30-0x00670C38 */
#include "common.h"
#include "protos/field.h"
#include "chr.h"        /* chrMotionList */
#include "main/n07.h"   /* FilePacMotionPack */

char *strcpy(char *, const char *);
void MemSet(void *, s32, s32);
void strcat(char *, char *);
s32 FldMemAlloc(s32);

void *FldLoadModelPack(char *name) {
    char buf[256];
    s32 size;
    s32 data;
    void *pk;

    MemSet(buf, 0, 0x100);
    strcpy(buf, "FIELD/");
    strcat(buf, name);
    size = FileGetSizeAligned(buf);
    if (size == 0) {
        return 0;
    }
    data = FldMemAlloc(size);
    FileRead(buf, data);
    pk = FilePacModelRelocate(data);
    FilePacModelBindTextures(pk);
    return pk;
}


FilePacMotionPack *FldLoadMotionPack(char *arg0) {
    char buf[256];
    s32 n;
    s32 p;
    FilePacMotionPack *h;
    u32 i;

    MemSet(buf, 0, 0x100);
    strcpy(buf, "FIELD/");
    strcat(buf, arg0);
    n = FileGetSizeAligned(buf);
    if (n == 0) {
        return 0;
    }
    p = FldMemAlloc(n);
    FileRead(buf, p);
    h = FilePacMotionRelocate(p);
    for (i = 0; i < h->count; i++) {
        chrMotionList.items[chrMotionList.count + i] = FilePacGetMotion(h, i);
    }
    chrMotionList.count = h->count;
    return h;
}
