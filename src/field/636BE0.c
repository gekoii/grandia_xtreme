#include "common.h"
#include "mc.h"
#include "save.h"
#include "sys.h"
#include "field/n24.h"
#include "protos/field.h"

typedef struct {
    s32 val;
    s16 w[4];
    u8 padC;
    u8 id;
    u8 padE[2];
} Ent;

typedef struct {
    Ent e[9];
} Tbl;

extern Tbl fldMenuLoadDecoTable;
extern s32 fldMenuLoadDecoCount;

void MemSet(void *, s32, s32);

void FldMenuLoadInit(void) {
    Tbl *t = &fldMenuLoadDecoTable;
    s16 *p;
    s32 i;
    s32 j;
    u8 *b0;
    u8 *b1;

    MemSet(t, 0, 0x8C);
    t->e[0].id = 0x27;
    t->e[1].id = 0xD;
    t->e[2].id = 0x31;
    t->e[3].id = 0x25;
    t->e[4].id = 0x71;
    t->e[5].id = 0x25;
    t->e[6].id = 0x15;
    t->e[7].id = 0x2D;
    /* entries 1-8 through byte pointers stepped by hand: t->e[i + 1] indexing uses fewer saved registers */
    b0 = (u8 *)&t->e[1].val;
    b1 = (u8 *)t->e[1].w;
    for (i = 0; i < 8; i++) {
        *(s32 *)(i * 16 + b0) = VuRandInt(0x100000);
        p = (s16 *)(i * 16 + (u32)b1);
        for (j = 0; j < 4; j++) {
            *p++ = VuRandInt(200);
        }
    }
    McLoadFlowInit();
    McGetPortState(0)->state = 0;
    McGetPortState(1)->state = 0;
    fldMenuSaveCardSlot = 0;
    fldMenuLoadDecoCount = 0;
    McSetSaveData(&sysSaveData, 0x3300);
    btlHandoff.softResetLock = 1;
}
