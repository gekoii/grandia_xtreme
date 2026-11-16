#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n24.h"

void *FldMemAlloc(s32);

void MiniConcertAlloc(void) {
    MiniConcertData **pc = &miniConcertData;
    MiniConcertWork **pd = &miniConcertWork;
    MiniConcertFx **pe = &miniConcertFx;

    *(s32 *)&miniConcertLayout = (s32)MiniGetLayout(); /* s32 stores: the pointer stores allocate registers differently */
    *(s32 *)&miniConcertUi = (s32)MiniGetUi();
    *pc = FldMemAlloc(0x94);
    *pd = FldMemAlloc(0x8CC0);
    *pe = FldMemAlloc(0x9810);
    sysSaveData.unk1F69 = 0x58;
    sysSaveData.unk1F6A = 2;
}
