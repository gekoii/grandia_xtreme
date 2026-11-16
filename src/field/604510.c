#include "common.h"
#include "field/n21.h"
#include "protos/field.h"

s32 AmapTemplatePick(AmapWork *w, s16 area, s16 idx) {
    s32 set;
    s32 t;
    s32 i;
    u8 id;
    u8 kind;

    if (amapForceAlgo1 != -1) {
        t = amapForceTemplates[amapForceTemplates[0] + 1];
        if (t != -1) {
            set = t;
            w->tmplLog[w->tmplLogCount].id = set;
            w->tmplLog[w->tmplLogCount].area = 0;
            w->tmplLog[w->tmplLogCount].list = 0;
            w->tmplLog[w->tmplLogCount].index = 0;
            amapForceTemplates[0]++;
            w->tmplLogCount++;
            return t;
        }
    }
    if (idx == -1) {
        for (;;) {
            set = 0;
            if (VuRandInt(100) + 1 <= 49) {
                set = 1;
            }
            if (w->tmplLists[area].cands[set][0].id == 0) {
                set++;
                set &= 1;
            }
            i = VuRandInt(w->tmplLists[area].cands[set][0].id) + 1;
            kind = w->tmplLists[area].cands[set][i].kind;
            id = w->tmplLists[area].cands[set][i].id;
            if (kind == 1) {
                if (!(w->tmplFlags & 1)) {
                    w->tmplFlags |= 1;
                    break;
                }
            } else if (kind == 2) {
                if (!(w->tmplFlags & 2)) {
                    w->tmplFlags |= 2;
                    break;
                }
            } else {
                break;
            }
        }
    } else {
        area = 9;
        if (idx < 11) {
            if (idx >= 0) {
                set = 0;
                i = idx + 1;
                goto found; /* goto + bare block: the original's layout (if/else is 8 insns shorter) */
            }
        }
        {
            set = 1;
            i = VuRandInt(w->tmplLists[area].cands[set][0].id) + 1;
        }
    found:
        id = w->tmplLists[area].cands[set][i].id;
    }
    w->tmplLog[w->tmplLogCount].id = id;
    w->tmplLog[w->tmplLogCount].area = area;
    w->tmplLog[w->tmplLogCount].list = set;
    w->tmplLog[w->tmplLogCount].index = i;
    w->tmplLogCount++;
    return id;
}
