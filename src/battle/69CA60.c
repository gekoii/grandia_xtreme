/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x006E0AB0-0x006E0AD4 */
#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "gph.h"
#include "party.h"
#include "battle/misc.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

typedef struct {
    f32 x, y, z, w;
} Vec4 __attribute__((aligned(16)));


f32 VuSinAngle(s32);

void BtlCmdMenuUpdate(void) {
    Pad *pad = padPort0;
    BtlCmdMenu *m;
    BtlCmdAreaReq *r;
    Vec4 v;
    Vec4 w;
    GphModel **op;
    s32 i;
    s32 n;
    s32 dbg;
    f32 one;
    f32 s;
    f32 x;
    f32 y;

    dbg = 0;
    m = &btlCmdMenu;
    BtlCmdMenuInput();
    if (m->state == 0) {
        return;
    }
    if (padPort0[0].held & 0x80) {
        if (m->unk14 < 0x2000) {
            BtlCamSetMode(6, 0x40, NULL, NULL);
            m->unk14 += 0x2000;
            IpTransStart(0, 0.0625f);
        }
    } else if (pad->released & 0x80) {
        m->unk14 &= 0xFF;
        IpTransStart(0, -0.0625f);
    }
    if (m->unk14 < 0x1000) {
        switch (m->unk14 & 0xFF) {
        case 2:
            BtlCamSetMode(2, 0x40, m->unit, NULL);
            break;
        case 3:
            BtlCamSetMode(3, 0x40, m->unit, NULL);
            break;
        case 8:
            BtlCamSetMode(8, 0x40, m->unit, NULL);
            break;
        case 6:
            BtlCamSetMode(6, 0x40, m->unit, NULL);
            break;
        case 0:
            BtlCamSetMode(0, 0x40, m->unit, BtlCmdMenuPointCamCb);
            break;
        }
        m->unk14 += 0x1000;
    }
    if (m->state == 0x10) {
        BtlCmdMenuRingUpdate(m);
    } else if (m->state == 0x41) {
        v.x = m->points[m->unkAC].x;
        v.z = m->points[m->unkAC].z;
        v.y = 60.0f;
        copyq(btlCmdMenuAllyCursor->pos, &v);
        btlCmdMenuAllyCursor->rot[1] += 0x200;
        UtilWorldToScreen(&v, &v);
        MenuCursorSetPos(v.x, v.y);
    } else if (m->state & 0x40) {
        BtlChrGetPartPos(&w, &btlUnits[m->unit->target], 1);
        if (m->unit->target < 4) {
            copyq(btlCmdMenuAllyCursor->pos, &w);
            btlCmdMenuAllyCursor->pos[1] = btlUnits[m->unit->target].params->colH + 40.0f;
            btlCmdMenuAllyCursor->rot[1] += 0x200;
        } else {
            copyq(btlCmdMenuEnemyCursor->pos, &w);
            btlCmdMenuEnemyCursor->rot[1] += 0x200;
        }
        UtilWorldToScreen(&w, &w);
        MenuCursorSetPos(w.x, w.y);
    }
    /* Debug code, dead once gcse propagates dbg = 0. Its size sets gcse's
       hash-table size, which orders the %hi registers of btlCmdMenuAllyCursor and
       btlCmdMenuEnemyCursor (s5/s4). */
    if (dbg) {
        MenuLrArrowsSet(1.0f, 2.0f, 3.0f);
        MenuLrArrowsSet(11.0f, 12.0f, 13.0f);
        MenuLrArrowsSet(21.0f, 22.0f, 23.0f);
        MenuLrArrowsSet(31.0f, 32.0f, 33.0f);
        MenuLrArrowsSet(41.0f, 42.0f, 43.0f);
        MenuLrArrowsSet(51.0f, 52.0f, 53.0f);
        MenuLrArrowsSet(61.0f, 62.0f, 63.0f);
        MenuLrArrowsSet(71.0f, 72.0f, 73.0f);
        MenuLrArrowsSet(81.0f, 82.0f, 83.0f);
        MenuLrArrowsSet(91.0f, 92.0f, 93.0f);
        MenuLrArrowsSet(101.0f, 102.0f, 103.0f);
    }
    ChrModelUvScrollUpdate(btlCmdMenuRingInst, 1.0f);
    r = &m->area;
    for (i = 0, op = btlCmdMenuPoints; i < 6; i++, op++) {
        ChrModelUvScrollUpdate(*op, 1.0f);
    }
    one = 1.0f;
    ChrModelUvScrollUpdate(btlCmdMenuAllyCursor, one);
    ChrModelUvScrollUpdate(btlCmdMenuEnemyCursor, one);
    switch (m->area.active) {
    case 1:
        r->scale += (1.3f - r->scale) * 0.1f;
        if (r->scale > 1.2f) {
            m->area.active = 2;
        }
        r->fade += 0.0625f;
        if (r->fade > one) {
            r->fade = one;
        }
        break;
    case 2:
        r->scale -= (1.3f - r->scale) * 0.1f;
        if (r->scale < one) {
            r->scale = one;
            m->area.active = 3;
        }
        break;
    case 4:
        r->fade -= 0.125f;
        r->scale += 0.1f;
        if (r->fade < 0.0f) {
            r->fade = 0.0f;
            m->area.active = 0;
        }
        break;
    }
    one = 1.0f;
    ChrModelUvScrollUpdate(btlCmdMenuCircleArea, one);
    ChrModelUvScrollUpdate(btlCmdMenuLineArea, one);
    ChrModelUvScrollUpdate(btlCmdMenuAllArea, one);
    if (m->unkB0 >= 0) {
    switch (m->unkB0) {
    case 0:
        m->unkB8 = 280.0f;
        m->unkBC = 130.0f;
        m->unkB0 = 1;
        break;
    case 1:
        break;
    case 16:
        m->unkB4 = 0;
        m->unkB0 = 17;
    case 17:
        n = ++m->unkB4;
        if (n < 10) {
            s = VuSinAngle(n * 0x666);
            m->unkB8 = s * -216.0f + 280.0f;
            m->unkBC = s * -10.0f + 130.0f;
        } else {
            m->unkB8 = 64.0f;
            m->unkBC = 120.0f;
            m->unkB0++;
        }
        break;
    }
    }
}
