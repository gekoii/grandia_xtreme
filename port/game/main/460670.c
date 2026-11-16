/* Port-only C for SndFsReadDir (0x00460670), which the decompilation still has in assembly. */
#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

extern s32 sndFsDirBuf;
extern s32 sndFsDirBufOfs;
/* The word after sndIopHeader (0x004A9518, no name of its own; the last
   word of the 0xC-byte sndIopStatusAddr object): entries read so far.
   Reached from sndIopStatusAddr, not as the word after sndIopHeader:
   sndIopHeader is a pointer, 8 bytes and an object of its own on x86-64
   (port/docs/win64.md) */
extern s32 sndIopStatusAddr[3];
#define sndFsEntryCount (sndIopStatusAddr[2])

void SndFsParseDirRecord(void *rec, SndFsDirEntry *e);
s32 SndFsReadDir(s32 lsn, s32 size, SndFsDirEntry *list);

/* Reads the ISO9660 directory at lsn (size bytes) into list, then each
   subdirectory's entries after it, recursively. The first two records are
   named "." and ".."; each directory's entries end with one of name "" and
   lsn -1. Returns the entries written, counting the subdirectories' end
   markers but not this directory's. The sectors are read through the
   uncached alias (| 0x20000000) of a 128-aligned slice of sndFsDirBuf. */
s32 SndFsReadDir(s32 lsn, s32 size, SndFsDirEntry *list) {
    s8 *buf;
    s8 *p;
    s8 *sector;
    s32 sectors;
    s32 count;
    s32 len;
    s32 n;
    SndFsDirEntry *cur;
    SndFsDirEntry *next;
    SndFsDirEntry *e;
    SndFsDirEntry *sub;

    buf = (s8 *)(((sndFsDirBuf + sndFsDirBufOfs + 0x7F) & 0x7FFFF80) | 0x20000000);
    sndFsDirBufOfs = sndFsDirBufOfs + 0x100 + size;
    sectors = (size >> 11) + ((size & 0x7FF) != 0);
    count = 0;
    cur = list;
    next = list + 1;
    while (SndFsReadRawSectors((s32)buf, lsn, sectors) < 0) {
    }
    while (sectors > 0) {
        sectors--;
        sector = buf;
        p = buf;
        while ((len = *p) > 0) {
            SndFsParseDirRecord(p, cur);
            if (count == 0) {
                cur->name[0] = '.';
                cur->name[1] = 0;
            }
            if (count == 1) {
                cur->name[2] = 0;
                cur->name[1] = '.';
                cur->name[0] = '.';
            }
            cur = next;
            p += len;
            cur->name[0] = 0;
            count++;
            next = cur + 1;
            sndFsEntryCount++;
        }
        buf = sector + 0x800;
    }
    cur->lsn = -1;
    sub = next;
    for (e = list; e->name[0] != 0; e++) {
        if (e->flags == 2 && e->name[0] != '.') {
            e->child = sub;
            n = SndFsReadDir(e->lsn, e->size, sub);
            sub += n;
            count += n + 1;
            sub->name[0] = 0;
            sub->lsn = -1;
            sub++;
        }
    }
    return count;
}
