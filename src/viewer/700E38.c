/* rodata: 0x007033D0-0x007033D8 */
#include "common.h"
#include "protos/viewer.h"
#include "sys.h"
#include "viewer/n35.h"

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
} Part;

typedef struct {
    u32 count;
    u32 nParts;
    u8 pad8[0x1C];
    Part **parts;
} Pack;

typedef struct {
    u8 pad0[4];
    s32 unk4;
} Ent;

char *strcpy(char *, const char *);
void MemSet(void *, s32, s32);
void strcat(char *, char *);

s32 VwrLoadModelPack(char *name, s32 count) {
    char buf[256];
    s32 size;
    s32 data;
    Pack *pk;
    u32 i;
    Ent *e;
    GphModel *obj;
    u32 n;

    MemSet(buf, 0, 0x100);
    strcpy(buf, "DATA/");
    strcat(buf, name);
    size = FileGetSizeAligned(buf);
    if (size == 0) {
        return 0;
    }
    if (count) {
        vwrStatFileBytes += size;
    }
    data = MemAlloc128(size);
    FileRead(buf, data);
    pk = FilePacModelRelocate(data);
    FilePacModelBindTextures(pk);
    if (sysIniViewerMip == 0) {
        for (i = 0; i < pk->nParts; i++) {
            pk->parts[i]->unk14 = 0;
        }
    }
    if (pk != NULL) {
        for (i = 0; i < pk->count; i++) {
            e = FilePacGetModel(pk, i);
            obj = ChrModelCreate(e);
            if (count) {
                vwrStatInstBytes += ChrModelCalcInstanceSize(e);
                vwrStatSortBytes += GphDrawListSize(e->unk4);
            }
            n = vwrModelList.count;
            vwrModelList.count = n + 1;
            vwrModelList.items[n] = obj;
        }
    }
    return data;
}

typedef struct {
    u32 count;
    s32 f4;
} Hdr_B;


s32 VwrLoadMotionPack(char *arg0) {
    char buf[256];
    s32 n;
    s32 p;
    Hdr_B *h;
    u32 i;

    MemSet(buf, 0, 0x100);
    strcpy(buf, "DATA/");
    strcat(buf, arg0);
    n = FileGetSizeAligned(buf);
    vwrStatFileBytes += n;
    if (n == 0) {
        return 0;
    }
    p = MemAlloc128(n);
    FileRead(buf, p);
    h = FilePacMotionRelocate(p);
    for (i = 0; i < h->count; i++) {
        chrMotionList.items[i] = FilePacGetMotion(h, i);
    }
    chrMotionList.count = h->count;
    return p;
}
