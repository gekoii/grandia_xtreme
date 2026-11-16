/* rodata: 0x004A15B8-0x004A15D8 */
#include "common.h"
#include "protos/main.h"
#include "ptcl.h"

typedef struct {
    u8 pad0[4];
    s32 unk4;
} Res;

char *strcpy(char *, const char *);
void strcat(char *, char *);

void PtclStarterInit(void) {
    char buf[0x100];
    s32 *pc;
    s32 file;
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 n2;
    PtclCtrl *r2;
    s32 q;
    s32 m;
    PtclStarterEntry *e2;
    PtclEffector *item;
    PtclEffector *dst;
    PtclCtrl *rec;
    PtclStarterEntry *ent;
    PtclStarterEntry *out;
    s8 *str;
    s32 o;

    pc = (s32 *)&ptclMainPool;
    *pc = MemCalloc16C(PtclSystemCalcSize(0x40, 0x20, 8));
    PtclSystemInit(*pc, 0x40, 0x20, 8);
    ptclPtbData = NULL;
    file = FileGetSizeAligned("DATA/PARTICLE.PTB");
    if (file == 0) {
        return;
    }
    ptclPtbData = MemAlloc128(file);
    FileRead("DATA/PARTICLE.PTB", ptclPtbData);
    ptclFileSize = file;

    item = (PtclEffector *)((u8 *)ptclPtbData + ptclPtbData->effectorOffset);
    n = ptclPtbData->effectorCount;
    for (; n > 0; n--, item++) {
        dst = PtclEffectorRegister(item->id);
        if (dst != NULL) {
            *dst = *item;
        }
    }

    n2 = ptclPtbData->ctrlCount;
    r2 = (PtclCtrl *)((u8 *)ptclPtbData + ptclPtbData->ctrlOffset);
    for (; n2 > 0; n2--, r2++) {
        PtclGenDef *sub;
        for (j = 0, sub = r2->keys; j < 4; j++, sub++) {
            for (k = 0; k < 4; k++) {
                sub->effectors[k] = PtclEffectorFindIndex(sub->effectors[k]);
            }
        }
    }

    e2 = (PtclStarterEntry *)((u8 *)ptclPtbData + ptclPtbData->starterOffset);
    out = ptclStarterTable;
    for (m = 0; m < ptclPtbData->starterCount && m < 16; m++) {
        *out++ = *e2++;
    }
    for (; m < 16; m++) {
        out->time = -1;
        out++;
    }

    str = (s8 *)ptclPtbData + ptclPtbData->stringOffset;
    for (i = 0; i < ptclPtbData->stringCount; i++) {
        strcpy(buf, "DATA/");
        switch (*str) {
        case 1:
            strcat(buf, str + 1);
            ptclTexIdTable[i] = UtilTexRegister((void *)UtilLoadFile(buf, 0));
            ptclAnimSetTable[i] = NULL;
            break;
        case 2:
            strcat(buf, str + 1);
            o = FilePacModelRelocate(UtilLoadFile(buf, 0));
            FilePacModelBindTextures(o);
            ptclModelTable[i] = FilePacGetModel(o, 0);
            break;
        case 3:
            strcat(buf, str + 1);
            ptclAnimSetTable[i] = UtilLoadFile(buf, 0);
            GphAnimSetRelocate(ptclAnimSetTable[i]);
            ptclTexIdTable[i] = UtilTexRegister(((GphAnimSet *)ptclAnimSetTable[i])->tex);
            break;
        }
        if (*str == 1 || *str == 3) {
            ptclFileSize += FileGetSizeAligned(buf);
        }
        if (*str != 0) {
            do {
                str++;
            } while (*str != 0);
        }
        str++;
    }

    ptclPeakWork = 0;
    ptclCurWork = 0;
    ptclSortAllocSize = 0;
    ent = (PtclStarterEntry *)((u8 *)ptclPtbData + ptclPtbData->starterOffset);
    for (q = 0; q < ptclPtbData->starterCount; q++, ent++) {
        if (ent->ctrl >= 0) {
            rec = (PtclCtrl *)((u8 *)ptclPtbData + ptclPtbData->ctrlOffset) + ent->ctrl;
            if (*PtclPtbGetString(ptclPtbData, rec->resIndex) == 2) {
                ptclSortAllocSize += ((Res *)ptclModelTable[rec->resIndex])->unk4 * rec->groupCount;
            }
        }
    }
    if (ptclSortAllocSize > 0) {
        ptclSortAllocSize = GphDrawListSize(ptclSortAllocSize);
    }
}
