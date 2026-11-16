/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "party.h"
#include "field/menu.h"

void ItemStatusResistBuild(s16 idx) {
    f32 *p;
    f32 f;
    f32 pct;
    s32 i;
    s32 n;
    s32 type;
    s32 base;
    s32 val;

    p = &ptyPartyUnits[idx].regMgcAttr[1];
    n = 0;
    for (i = 1; i < 9; i++) {
        f = *p;
        pct = 1.0f;
        if (f > pct) {
            pct = -((f - pct) * 100.0f + 0.5f);
        } else if (f == pct) {
            pct = 0.0f;
        } else {
            pct = -(f - pct) * 100.0f + 0.5f;
        }
        type = 0;
        if (pct < -100.0f || pct < 0.0f) {
            base = 0;
            val = 100;
        } else if ((s32)pct == 0) {
            type = 1;
            val = pct + 100.0f;
            base = 100;
        } else if (pct < 100.0f) {
            type = 2;
            val = pct + 100.0f;
            base = 100;
        } else if (pct < 150.0f) {
            type = 3;
            val = pct + 100.0f;
            base = 100;
        } else if (pct < 200.0f) {
            type = 4;
            val = pct + 100.0f;
            base = 100;
        } else {
            type = 4;
            base = 100;
            val = 300;
        }
        p++;
        itemWork->resists[n].id = i;
        itemWork->resists[n].type = type;
        itemWork->resists[n].base = base;
        itemWork->resists[n].val = val;
        itemWork->resists[n].pct = pct;
        n++;
    }
}
