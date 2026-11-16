#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "field/menu.h"
#include "field/n24.h"
#include "main/n03.h"

extern FldMenuQuad *fldMenuSaveSprFaces;
extern FldMenuQuad *fldMenuSaveSprLabels;
extern FldMenuQuad *fldMenuSaveSprNumbers;
extern FldMenuQuad *fldMenuSaveQuads;
extern s8 fldMenuSaveCardHasSpace;

s32 FldMenuWinTextAdd(u8, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void FldMenuSaveFileRowBuild(s32 arg0, s32 arg1, s32 arg2, McSlot *rec, s32 arg4, s32 arg5, s32 arg6) {
    s32 font;
    s32 i;
    FldMenuQuad *p;

    font = sysGrxsysPac->msg->tbl[11];
    if (arg6 == 0) {
        switch (rec->unk2) {
        case 0:
            fldMenuSaveQuads->type = 7;
            fldMenuSaveQuads->v[0].r = 0;
            fldMenuSaveQuads->v[0].g = 0x40;
            fldMenuSaveQuads->v[0].b = 0;
            fldMenuSaveQuads->v[0].a = 0;
            fldMenuSaveQuads->v[1].r = 0;
            fldMenuSaveQuads->v[1].g = 0x40;
            fldMenuSaveQuads->v[1].b = 0;
            fldMenuSaveQuads->v[1].a = 0;
            fldMenuSaveQuads->v[2].r = 0;
            fldMenuSaveQuads->v[2].g = 0x20;
            fldMenuSaveQuads->v[2].b = 0;
            fldMenuSaveQuads->v[2].a = 0;
            fldMenuSaveQuads->v[3].r = 0;
            fldMenuSaveQuads->v[3].g = 0x20;
            fldMenuSaveQuads->v[3].b = 0;
            fldMenuSaveQuads->v[3].a = 0;
            fldMenuSaveQuads->v[0].x = arg1 - 8;
            fldMenuSaveQuads->v[0].y = arg2;
            fldMenuSaveQuads->v[1].x = arg1 + 0x1F4;
            fldMenuSaveQuads->v[1].y = arg2;
            fldMenuSaveQuads->v[2].x = arg1 - 8;
            fldMenuSaveQuads->v[2].y = arg2 + 0x14;
            fldMenuSaveQuads->v[3].x = arg1 + 0x1F4;
            fldMenuSaveQuads->v[3].y = arg2 + 0x14;
            fldMenuSaveQuads++;
            fldMenuSaveQuads->type = 6;
            fldMenuSaveQuads->v[0].r = 0x20;
            fldMenuSaveQuads->v[0].g = 0;
            fldMenuSaveQuads->v[0].b = 0x20;
            fldMenuSaveQuads->v[0].a = 0x80;
            fldMenuSaveQuads->v[1].r = 0x20;
            fldMenuSaveQuads->v[1].g = 0;
            fldMenuSaveQuads->v[1].b = 0x30;
            fldMenuSaveQuads->v[1].a = 0x80;
            fldMenuSaveQuads->v[2].r = 0x20;
            fldMenuSaveQuads->v[2].g = 0;
            fldMenuSaveQuads->v[2].b = 0x40;
            fldMenuSaveQuads->v[2].a = 0x80;
            fldMenuSaveQuads->v[3].r = 0x20;
            fldMenuSaveQuads->v[3].g = 0;
            fldMenuSaveQuads->v[3].b = 0x40;
            fldMenuSaveQuads->v[3].a = 0x80;
            fldMenuSaveQuads->v[0].x = arg1 - 8;
            fldMenuSaveQuads->v[0].y = arg2;
            fldMenuSaveQuads->v[1].x = arg1 + 0x1F4;
            fldMenuSaveQuads->v[1].y = arg2;
            fldMenuSaveQuads->v[2].x = arg1 - 8;
            fldMenuSaveQuads->v[2].y = arg2 + 0x14;
            fldMenuSaveQuads->v[3].x = arg1 + 0x1F4;
            fldMenuSaveQuads->v[3].y = arg2 + 0x14;
            fldMenuSaveQuads++;
            break;
        case 1:
            fldMenuSaveQuads->type = 7;
            fldMenuSaveQuads->v[0].r = 0;
            fldMenuSaveQuads->v[0].g = 0x40;
            fldMenuSaveQuads->v[0].b = 0;
            fldMenuSaveQuads->v[0].a = 0;
            fldMenuSaveQuads->v[1].r = 0;
            fldMenuSaveQuads->v[1].g = 0x40;
            fldMenuSaveQuads->v[1].b = 0;
            fldMenuSaveQuads->v[1].a = 0;
            fldMenuSaveQuads->v[2].r = 0;
            fldMenuSaveQuads->v[2].g = 0x20;
            fldMenuSaveQuads->v[2].b = 0;
            fldMenuSaveQuads->v[2].a = 0;
            fldMenuSaveQuads->v[3].r = 0;
            fldMenuSaveQuads->v[3].g = 0x20;
            fldMenuSaveQuads->v[3].b = 0;
            fldMenuSaveQuads->v[3].a = 0;
            fldMenuSaveQuads->v[0].x = arg1 - 8;
            fldMenuSaveQuads->v[0].y = arg2;
            fldMenuSaveQuads->v[1].x = arg1 + 0x1F4;
            fldMenuSaveQuads->v[1].y = arg2;
            fldMenuSaveQuads->v[2].x = arg1 - 8;
            fldMenuSaveQuads->v[2].y = arg2 + 0x14;
            fldMenuSaveQuads->v[3].x = arg1 + 0x1F4;
            fldMenuSaveQuads->v[3].y = arg2 + 0x14;
            fldMenuSaveQuads++;
            fldMenuSaveQuads->type = 6;
            fldMenuSaveQuads->v[0].r = 0x60;
            fldMenuSaveQuads->v[0].g = 0;
            fldMenuSaveQuads->v[0].b = 0;
            fldMenuSaveQuads->v[0].a = 0x80;
            fldMenuSaveQuads->v[1].r = 0x50;
            fldMenuSaveQuads->v[1].g = 0;
            fldMenuSaveQuads->v[1].b = 0;
            fldMenuSaveQuads->v[1].a = 0x80;
            fldMenuSaveQuads->v[2].r = 0x70;
            fldMenuSaveQuads->v[2].g = 0;
            fldMenuSaveQuads->v[2].b = 0;
            fldMenuSaveQuads->v[2].a = 0x80;
            fldMenuSaveQuads->v[3].r = 0x70;
            fldMenuSaveQuads->v[3].g = 0;
            fldMenuSaveQuads->v[3].b = 0;
            fldMenuSaveQuads->v[3].a = 0x80;
            fldMenuSaveQuads->v[0].x = arg1 - 8;
            fldMenuSaveQuads->v[0].y = arg2;
            fldMenuSaveQuads->v[1].x = arg1 + 0x1F4;
            fldMenuSaveQuads->v[1].y = arg2;
            fldMenuSaveQuads->v[2].x = arg1 - 8;
            fldMenuSaveQuads->v[2].y = arg2 + 0x14;
            fldMenuSaveQuads->v[3].x = arg1 + 0x1F4;
            fldMenuSaveQuads->v[3].y = arg2 + 0x14;
            fldMenuSaveQuads++;
            break;
        }
    } else {
        fldMenuSaveQuads->type = 7;
        fldMenuSaveQuads->v[0].r = 0;
        fldMenuSaveQuads->v[0].g = 0x40;
        fldMenuSaveQuads->v[0].b = 0;
        fldMenuSaveQuads->v[0].a = 0;
        fldMenuSaveQuads->v[1].r = 0;
        fldMenuSaveQuads->v[1].g = 0x40;
        fldMenuSaveQuads->v[1].b = 0;
        fldMenuSaveQuads->v[1].a = 0;
        fldMenuSaveQuads->v[2].r = 0;
        fldMenuSaveQuads->v[2].g = 0x20;
        fldMenuSaveQuads->v[2].b = 0;
        fldMenuSaveQuads->v[2].a = 0;
        fldMenuSaveQuads->v[3].r = 0;
        fldMenuSaveQuads->v[3].g = 0x20;
        fldMenuSaveQuads->v[3].b = 0;
        fldMenuSaveQuads->v[3].a = 0;
        fldMenuSaveQuads->v[0].x = arg1 - 8;
        fldMenuSaveQuads->v[0].y = arg2;
        fldMenuSaveQuads->v[1].x = arg1 + 0x1F4;
        fldMenuSaveQuads->v[1].y = arg2;
        fldMenuSaveQuads->v[2].x = arg1 - 8;
        fldMenuSaveQuads->v[2].y = arg2 + 0x14;
        fldMenuSaveQuads->v[3].x = arg1 + 0x1F4;
        fldMenuSaveQuads->v[3].y = arg2 + 0x14;
        fldMenuSaveQuads++;
        fldMenuSaveQuads->type = 6;
        fldMenuSaveQuads->v[0].r = 0x70;
        fldMenuSaveQuads->v[0].g = 0x20;
        fldMenuSaveQuads->v[0].b = 0x30;
        fldMenuSaveQuads->v[0].a = 0x80;
        fldMenuSaveQuads->v[1].r = 0;
        fldMenuSaveQuads->v[1].g = 0x30;
        fldMenuSaveQuads->v[1].b = 0x30;
        fldMenuSaveQuads->v[1].a = 0x80;
        fldMenuSaveQuads->v[2].r = 0x80;
        fldMenuSaveQuads->v[2].g = 0x30;
        fldMenuSaveQuads->v[2].b = 0x40;
        fldMenuSaveQuads->v[2].a = 0x80;
        fldMenuSaveQuads->v[3].r = 0;
        fldMenuSaveQuads->v[3].g = 0x40;
        fldMenuSaveQuads->v[3].b = 0x30;
        fldMenuSaveQuads->v[3].a = 0x80;
        fldMenuSaveQuads->v[0].x = arg1 - 8;
        fldMenuSaveQuads->v[0].y = arg2;
        fldMenuSaveQuads->v[1].x = arg1 + 0x1F4;
        fldMenuSaveQuads->v[1].y = arg2;
        fldMenuSaveQuads->v[2].x = arg1 - 8;
        fldMenuSaveQuads->v[2].y = arg2 + 0x14;
        fldMenuSaveQuads->v[3].x = arg1 + 0x1F4;
        fldMenuSaveQuads->v[3].y = arg2 + 0x14;
        fldMenuSaveQuads++;
    }
    if (arg4 >= 0) {
        FldMenuWinTextAdd(arg0, 1, font, arg1, arg2 + 2, 0x10, 0x10, arg5++, arg4 + 10, 0);
        if (rec->used == 0) {
            if (fldMenuSaveIsLoad == 0) {
                if (fldMenuSaveCardHasSpace != 0) {
                    FldMenuWinTextAdd(arg0, 1, font, arg1 + 0x94, arg2 + 0x20, 0x18, 0x14, arg5, 9, 2);
                } else {
                    FldMenuWinTextAdd(arg0, 1, font, arg1 + 0x94, arg2 + 0x20, 0x18, 0x14, arg5, 8, 0);
                }
            } else {
                FldMenuWinTextAdd(arg0, 1, font, arg1 + 0x94, arg2 + 0x20, 0x18, 0x14, arg5, 9, 2);
            }
            return;
        }
        if (rec->status == -1) {
            FldMenuWinTextAdd(arg0, 1, font, arg1 + 0x80, arg2 + 0x20, 0x18, 0x14, arg5, 7, 2);
            return;
        }
    } else {
        FldMenuWinTextAdd(arg0, 1, font, arg1, arg2 + 2, 0x10, 0x10, arg5++, 2, 0);
    }
    FldMenuWinTextAdd(arg0, 1, font, arg1 + 0x158, arg2 + 2, 0x10, 0x10, arg5++, 6, 0);
    FldMenuWinTextAdd(arg0, 1, font, arg1 + 0x1CF, arg2 + 1, 0x10, 0x10, arg5, 0x78, 0);
    /* cast calls: FldMenuPlayTimeBuild with the floats declared before the u8
     * (EABI loads arguments in declaration order); FldMenuSaveFilePartyBuild
     * called directly stores fldMenuSaveSprFaces before the partyCount load */
    fldMenuSaveSprNumbers = ((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuPlayTimeBuild)(fldMenuSaveSprNumbers, rec->playTime, 0, arg1 + 0x1AE, arg2 + 2, 0.8f, 0.8f, 0x80);
    fldMenuSaveSprFaces = ((FldMenuQuad *(*)(FldMenuQuad *, McSlot *, s32, s32))FldMenuSaveFilePartyBuild)(fldMenuSaveSprFaces, rec, arg1, arg2);
    for (i = 0; i < rec->partyCount; i++) {
        fldMenuSaveSprLabels->type = 1;
        fldMenuSaveSprLabels->v[0].r = 0x80;
        fldMenuSaveSprLabels->v[0].g = 0x80;
        fldMenuSaveSprLabels->v[0].b = 0x80;
        fldMenuSaveSprLabels->v[0].a = 0x80;
        p = fldMenuSaveSprLabels;
        p->v[0].x = arg1 + 0x40 + i * 0x78;
        p->v[0].y = arg2 + 0x34;
        p->v[1].x = arg1 + (i * 0x78 + 0x5E);
        p->v[1].y = arg2 + 0x48;
        p->v[0].u = 0xEC;
        p->v[0].v = 0x5C;
        p->v[1].u = 0x100;
        p->v[1].v = 0x6A;
        fldMenuSaveSprLabels = p + 1;
        p->z = 0;
        /* cast calls: FldMenuPlayTimeBuild with the floats declared before the u8
     * (EABI loads arguments in declaration order); FldMenuSaveFilePartyBuild
     * called directly stores fldMenuSaveSprFaces before the partyCount load */
    fldMenuSaveSprNumbers = ((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberBuild)(fldMenuSaveSprNumbers, rec->level[i], 0, arg1 + 0x60 + i * 0x78, arg2 + 0x2E, 0, 1.1f, 1.2f);
    }
}
