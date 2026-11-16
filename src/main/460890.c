#include "common.h"
#include "main/n10.h"

typedef struct {
    u8 pad0[6];
    s8 lsn0;
    u8 lsn1;
    u8 lsn2;
    u8 lsn3;
    u8 padA[0xE - 0xA];
    s8 size0;
    u8 size1;
    u8 size2;
    u8 size3;
    u8 pad12[0x19 - 0x12];
    u8 flags;
    u8 pad1A[0x20 - 0x1A];
    s8 namelen;
    s8 name[1];
} DirRec;

void SndFsParseDirRecord(DirRec *rec, SndFsDirEntry *e) {
    s32 i;
    s8 *p;

    for (i = 13, p = &e->name[13]; i >= 0; i--, p--) {
        *p = 0;
    }
    for (i = 0; i < rec->namelen; i++) {
        e->name[i] = *(i + rec->name);
        if (e->name[i] == 0) {
            break;
        }
        if (e->name[i] == ';') {
            e->name[i] = 0;
            break;
        }
    }
    i = rec->lsn0 << 24;
    i += rec->lsn1 << 16;
    i += rec->lsn2 << 8;
    i += rec->lsn3;
    e->lsn = i;
    i = rec->size0 << 24;
    i += rec->size1 << 16;
    i += rec->size2 << 8;
    i += rec->size3;
    e->size = i;
    e->flags = (s8)rec->flags;
    e->child = NULL;
    e->unk1C = 0;
}
