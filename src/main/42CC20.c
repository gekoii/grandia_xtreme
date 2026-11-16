#include "common.h"
#include "main/n03.h"

extern s32 strcmp(char *, char *);
extern char mcIconFileName[];
extern char mcIconSysName[];
extern char mcSaveDirName[];

s32 McGameFilesKB(McDirList *d) {
    s32 total;
    s32 i;
    sceMcTblGetDir *e;
    char *name;

    i = 0;
    total = 0;
    /* two nested do-while(0)s: allocation weight, total gets s2 and i s3 (unwrapped they swap) */
    do {
        do {
            for (; i < d->count; i++) {
                e = &d->entries[i];
                name = e->EntryName;
                if (strcmp(name, mcIconSysName) == 0 || strcmp(name, mcIconFileName) == 0 || strcmp(name, mcSaveDirName) == 0) {
                    total += (e->FileSizeByte + 0x3FF) >> 10;
                }
            }
            return total + 2;
        } while (0);
    } while (0);
}
