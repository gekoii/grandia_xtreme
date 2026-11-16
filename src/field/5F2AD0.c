#include "common.h"
#include "protos/field.h"
#include "party.h"

typedef struct {
    s16 v[8][6];
} Preview;


s32 PtyEquipPreviewStats(s16 id, s16 item, Preview *out) {
    s16 save[6];
    s32 i;
    PtyItem *d;

    PtyUnitInitFromChar(ptyCharCalcUnits, id + 0x1001);
    PtyUnitCalcParams(ptyCharCalcUnits, 0, 0);
    for (i = 0; i < 48; i++) {
        ((s16 *)out)[i] = 0;
    }
    /* volatile store: a plain one gets combined and scheduled differently (2 insns fewer) */
    *(volatile s16 *)&out->v[0][0] = out->v[1][0] = ptyCharCalcUnits->params[0];
    out->v[1][1] = out->v[0][1] = ptyCharCalcUnits->params[1];
    out->v[1][2] = out->v[0][2] = ptyCharCalcUnits->params[2];
    out->v[1][3] = out->v[0][3] = ptyCharCalcUnits->params[3];
    out->v[1][4] = out->v[0][4] = ptyCharCalcUnits->params[4];
    out->v[1][5] = out->v[0][5] = ptyCharCalcUnits->params[5];
    if (item < 0) {
        return 0;
    }
    for (i = 0; i < 6; i++) {
        save[i] = ptyCharData[id].equip[i];
    }
    d = (PtyItem *)((item << 5) + (u32)ptyItemTable);
    if (d->type == 0xE || d->equip == NULL) {
    fail:
        return -1;
    }
    if (!((d->equip->charMask >> id) & 1)) {
        goto fail; /* goto: a joined || condition changes the branch layout */
    }
    switch (d->type) {
    case 5:
        ptyCharData[id].equip[0] = item;
        break;
    case 6:
        ptyCharData[id].equip[1] = item;
        break;
    case 7:
        ptyCharData[id].equip[2] = item;
        break;
    case 8:
        ptyCharData[id].equip[3] = item;
        break;
    case 9:
        ptyCharData[id].equip[4] = item;
        break;
    case 10:
        ptyCharData[id].equip[5] = item;
        break;
    }
    PtyUnitCalcParams(&ptyCharCalcUnits[id], 0, 0);
    out->v[1][0] = ptyCharCalcUnits->params[0];
    out->v[1][1] = ptyCharCalcUnits->params[1];
    out->v[1][2] = ptyCharCalcUnits->params[2];
    out->v[1][3] = ptyCharCalcUnits->params[3];
    out->v[1][4] = ptyCharCalcUnits->params[4];
    out->v[1][5] = ptyCharCalcUnits->params[5];
    for (i = 0; i < 6; i++) { /* flat indexes: out->v[2][i] = ... adds insns */
        ((s16 *)out)[i + 12] = ((s16 *)out)[i + 6] - ((s16 *)out)[i];
    }
    for (i = 0; i < 6; i++) {
        ptyCharData[id].equip[i] = save[i];
    }
    return 0;
}
