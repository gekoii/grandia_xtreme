#include "common.h"
#include "party.h"

/* Turns a file offset into a pointer: adds the table base when the value is
 * below it (not yet relocated). */
#define RELOC(x, b) { u32 v_ = (u32)(x); u32 k_ = v_ < (u32)(b); v_ = k_ * (u32)(b) + v_; (x) = (__typeof__(x))v_; }

/* PtyMoveLevelData with effects as a u32: its store must stay in the same
 * alias set as the s32 name loads after the loop (a pointer store lets them
 * move above it) */
typedef struct {
    u8 pad[0x10];
    u32 effects;
    s8 effectCount;
    u8 pad15[3];
} LevelData;

void PtySysTablesRelocate(void) {
    s32 i;
    s32 j;
    s32 n;

    {
        PtyCharDef *p = ptyCharDefTable;
        u32 base1 = (u32)p;
        for (i = 8; i > 0; i--) {
            RELOC(p->traces, base1);
            RELOC(p->name, base1);
            p++;
        }
    }
    {
        PtyItem *p = ptyItemTable;
        u32 base2 = (u32)p;
        for (i = 0x800; i > 0; i--) {
            if (p->equip != 0) {
                RELOC(p->equip, base2);
            }
            if (p->use != 0) {
                RELOC(p->use, ptyItemTable);
                if (p->use->effectCount != 0) {
                    RELOC(p->use->effects, ptyItemTable);
                }
            }
            RELOC(p->name, base2);
            RELOC(p->help, base2);
            RELOC(p->helpDesc, base2);
            p++;
        }
    }
    {
        PtySkillDef *p = ptySkillTable;
        u32 base3 = (u32)p;
        for (i = 0x97; i > 0; i--) {
            RELOC(p->name, base3);
            RELOC(p->help, base3);
            RELOC(p->helpDesc, base3);
            p++;
        }
    }
    {
        PtyMove *p = ptyMoveTable;
        u32 base4 = (u32)p;
        for (i = 0x12D; i > 0; i--) {
            n = p->levelCount;
            if (n > 0) {
                LevelData *e;
                RELOC(p->levels, base4);
                e = (LevelData *)p->levels;
                for (j = n; j > 0; j--) {
                    if (e->effectCount > 0) {
                        RELOC(e->effects, ptyMoveTable);
                    }
                    e++;
                }
            }
            RELOC(p->name, base4);
            RELOC(p->help, base4);
            RELOC(p->helpDesc, base4);
            p++;
        }
    }
}
