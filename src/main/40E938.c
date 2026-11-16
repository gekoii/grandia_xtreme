#include "common.h"
#include "main/n01.h"

/* chrPortraitIndexTable is reached as a member of a struct at ptySkillTable
 * (0x004A9480 + 0xC): the loop then reuses arg0 for its count instead of
 * reloading the global, which a plain pointer global doesn't allow. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    ChrPortraitIndex *portraits;
} Glob0040E938;

extern Glob0040E938 ptySkillTable;

void ChrPortraitTableInit(ChrPortraitIndex *arg0) {
    u32 i;
    s32 *p;

    ptySkillTable.portraits = arg0;
    arg0->entries = (ChrPortraitEntry *)((s32)arg0 + (s32)arg0->entries);
    p = (s32 *)((s32)arg0 + (s32)arg0->names);
    arg0->names = p;
    for (i = 0; i < ptySkillTable.portraits->nameCount; i++) {
        *p += (s32)arg0;
        p++;
    }
}
