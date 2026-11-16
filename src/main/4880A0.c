/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

void __sread(void);
void __sseek(void);
void __sclose(void);

typedef struct S {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    struct S *unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 pad[0x54 - 0x30];
    s32 unk54;
} S;

void std(S *arg0, s16 arg1, s16 arg2, s32 arg3) {
    arg0->unk54 = arg3;
    arg0->unkC = arg1;
    arg0->unkE = arg2;
    arg0->unk20 = __sread;
    arg0->unk24 = __swrite;
    arg0->unk28 = __sseek;
    arg0->unk2C = __sclose;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unk10 = 0;
    arg0->unk18 = 0;
    arg0->unk1C = arg0;
    arg0->unk0 = 0;
}
