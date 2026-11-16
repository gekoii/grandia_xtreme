#include "common.h"
#include "protos/main.h"
#include "party.h"
#include "main/n02.h"

typedef struct {
    u8 unk0;
    u8 pad[0xF];
} Ctx;

extern s8 menuHelpWeaponIcons[2];
extern u8 menuHelpElement;

void MenuHelpBuildWeapon(void) {
    Ctx ctx;
    PtyItem *party;
    PtyEquipData *st;
    s8 n;
    s8 m;
    MenuHelp *g;

    g = &menuHelp;
    n = 0;
    party = g->data;
    st = party->equip;
    ctx.unk0 = 0;
    if (g->style == 1) {
        if (MenuHelpAddStats(6, 6) != 0) {
            g->lines[n] = 4;
            n++;
        }
        if (n < 2) {
            if (st->unkF != 0) {
                g->lines[n] = 6;
                MenuHelpAddElement(&ctx, 6, n * 24 + 6);
                n++;
            }
            menuHelpElement = st->unkF;
            if (n < 2) {
                if (st->unk14 > 0) {
                    g->lines[n] = 7;
                    MenuHelpAddStatusEffect(&ctx, 6, (s16)(n * 24 + 6));
                    n++;
                }
                if (n < 2) {
                    if (st->unkE != 0) {
                        g->lines[n] = 8;
                        MenuHelpAddStrongVs(&ctx, 6, (s16)(n * 24 + 6));
                        n++;
                    }
                    if (n < 2) {
                        if (st->effectType != 0) {
                            g->lines[n] = 5;
                            MenuHelpAddEffectLabel(&ctx, 5, 6, (s16)(n * 24 + 6));
                            n++;
                        }
                        if (n < 2) {
                            if (party->use != 0) {
                                g->lines[n] = 9;
                                MenuHelpAddEffectLabel(&ctx, 9, 6, (s16)(n * 24 + 6));
                                n++;
                            }
                        }
                    }
                }
            }
        }
    } else {
        MenuHelpAddName(&ctx, 6, 6);
        g->lines[n] = 1;
        n++;
        if (MenuHelpAddStats(0x2E, n * 24 + 6) != 0) {
            g->lines[n] = 4;
            n++;
        }
        m = 0;
        if (st->unkF != 0) {
            MenuHelpAddElement(&ctx, (s16)(m * 0xA0 + 0x2E), n * 24 + 6);
            menuHelpWeaponIcons[m] = 1;
            m++;
        }
        menuHelpElement = st->unkF;
        if (st->unkE != 0) {
            MenuHelpAddStrongVs(&ctx, (s16)(m * 0xA0 + 0x2E), n * 24 + 6);
            menuHelpWeaponIcons[m] = 2;
            m++;
        }
        if (st->unk14 > 0 && m < 2) {
            MenuHelpAddStatusEffect(&ctx, (s16)(m * 0xA0 + 0x2E), n * 24 + 6);
            menuHelpWeaponIcons[m] = 3;
            m++;
        }
        if (m > 0) {
            g->lines[n] = 5;
            n++;
            for (; m < 2; m++) {
                menuHelpWeaponIcons[m] = 0;
            }
        }
        if (*(u8 *)((PtyItem *)g->data)->helpDesc != 0) {
            MenuHelpAddDesc(&ctx, 0x2E, (s16)(n * 24 + 6));
            g->lines[n] = 2;
            n++;
        }
        if (n >= 4) {
            return;
        }
        if (party->use != 0) {
            MenuHelpAddLinkedSkill(&ctx, party->use, 0x2E, 0x4E);
            g->lines[n] = 9;
            n++;
        }
    }
    for (; n < 4; n++) {
        g->lines[n] = 0;
    }
}
