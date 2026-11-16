/* rodata: 0x004A5A30-0x004A5A70 */
#include "common.h"

typedef struct {
    u8 Head[4];
    u16 Reserv1;
    u16 OffsLF;
    u32 Reserv2;
    u32 TransRate;
    s32 BgColor[4][4];
    f32 LightDir[3][4];
    f32 LightColor[3][4];
    f32 Ambient[4];
    char TitleName[68];
    char FnameView[64];
    char FnameCopy[64];
    char FnameDel[64];
    u8 Reserve3[512];
} IconSys;

void strncpy(void *, char *, s32);

extern IconSys *mcIconSysData;
extern s32 mcBlankSaveData[256];

void McIconSysBuild(void) {
    IconSys *p = mcIconSysData;
    s32 i;

    strncpy(p->Head, "PS2D", 4);
    p->OffsLF = 0x1C;
    p->TransRate = 0x3C;
    p->Reserv1 = 0;
    p->BgColor[0][0] = 0;
    p->BgColor[0][1] = 0;
    p->BgColor[0][2] = 0;
    p->BgColor[0][3] = 0;
    p->BgColor[1][0] = 0;
    p->BgColor[1][1] = 0;
    p->BgColor[1][2] = 0;
    p->BgColor[1][3] = 0;
    p->BgColor[2][0] = 0;
    p->BgColor[2][1] = 0;
    p->BgColor[2][2] = 0;
    p->BgColor[2][3] = 0;
    p->BgColor[3][0] = 0;
    p->BgColor[3][1] = 0;
    p->BgColor[3][2] = 0;
    p->BgColor[3][3] = 0;
    p->LightDir[0][0] = 0;
    p->LightDir[0][1] = 0;
    p->LightDir[0][2] = 1.0f;
    p->LightDir[0][3] = 0;
    p->LightDir[1][0] = 0;
    p->LightDir[1][1] = 0;
    p->LightDir[1][2] = 0;
    p->LightDir[1][3] = 0;
    p->LightDir[2][0] = 0;
    p->LightDir[2][1] = 0;
    p->LightDir[2][2] = 0;
    p->LightDir[2][3] = 0;
    p->LightColor[0][0] = 1.0f;
    p->LightColor[0][1] = 1.0f;
    p->LightColor[0][2] = 1.0f;
    p->LightColor[0][3] = 0;
    p->LightColor[1][0] = 0;
    p->LightColor[1][1] = 0;
    p->LightColor[1][2] = 0;
    p->LightColor[1][3] = 0;
    p->LightColor[2][0] = 0;
    p->LightColor[2][1] = 0;
    p->LightColor[2][2] = 0;
    p->LightColor[2][3] = 0;
    p->Ambient[0] = 0.4f;
    p->Ambient[1] = 0.4f;
    p->Ambient[2] = 0.4f;
    p->Ambient[3] = 0.4f;
    __builtin_strcpy(p->TitleName, "\x82\x66\x82\x71\x82\x60\x82\x6d\x82\x63\x82\x68\x82\x60\x81\x40\x82\x77\x82\x73\x82\x71\x82\x64\x82\x6c\x82\x64\x81\x40\x82\x63\x82\x60\x82\x73\x82\x60");
    __builtin_strcpy(p->FnameView, "icon0.ico");
    __builtin_strcpy(p->FnameCopy, "icon0.ico");
    __builtin_strcpy(p->FnameDel, "icon0.ico");
    for (i = 0; i < 512; i++) {
        p->Reserve3[i] = 0;
    }
    for (i = 0; i < 256; i++) {
        mcBlankSaveData[i] = 0xFF;
    }
}
