#include "common.h"
#include "field/n26.h"

s32 strcmp(const char *, const char *);

sceMcTblGetDir *McSaveFindDirEntry(char *name) {
    sceMcTblGetDir *e;
    s32 i;

    e = mcDirListPtr->entries;
    for (i = 0; i < mcDirListPtr->count; i++, e++) {
        if (strcmp(e->EntryName, name) == 0) {
            return e;
        }
    }
    return NULL;
}
