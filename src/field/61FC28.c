/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x00674F90-0x00675020 */
#include "common.h"
#include "protos/field.h"
#include "grxsys.h"
#include "pad.h"
#include "field/menu.h"
#include "field/n23.h"

/* 0x0061FC28, 0x0061FEC8 and 0x00620530 share one .rodata section: 0x00620530's
   switch table at 0x00674FD0 is only 16-aligned when the section starts with
   0x0061FC28's table at 0x00674F90. */

typedef struct {
    u8 u, v;
} UV;

extern UV fldMenuCommandIconUVs[];

s32 FldMenuWinTextAdd(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* memset by its address name: called as memset, gcc expands the 4-byte clear inline */
void func_0048281C(void *, s32, s32);

s32 FldMenuTopUpdate(s32 arg0) {
    s32 ret;
    s32 w;
    u16 trig;

    ret = 0;
    FldMenuGuideSet(1);
    if (fldMenuWork->unk6 == 0) {
        trig = padPort0[0].trigger;
        if (trig & 0x8000) {
            if (fldMenuWork->topCursor != 0) {
                fldMenuWork->topCursor--;
            } else {
                fldMenuWork->topCursor = 5;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            return 0;
        }
        if (trig & 0x2000) {
            if (fldMenuWork->topCursor < 5) {
                fldMenuWork->topCursor++;
            } else {
                fldMenuWork->topCursor = 0;
            }
            SndSePlay(0x7D1, 0, 0, 0x1E, 0);
            return 0;
        }
    }
    FldMenuWinTextClear(0x11);
    w = TxtMsgGetWidth(sysGrxsysPac->msg->tbl[0], fldMenuWork->topCursor + 6, 0x18);
    FldMenuWinTextAdd(0x11, 5, sysGrxsysPac->msg->tbl[0], -0x5C, -0x14, 0x438 / w, 0x18, 0, fldMenuWork->topCursor + 6, 2);
    if (FldMenuWinGetState((u8)arg0) != 4) {
        return 0;
    }
    if (fldMenuWork->unk6 != 0) {
        FldMenuPartySelect(2, &fldMenuWork->unk18, &fldMenuWork->unk1C);
        FldMenuPartyPanelUpdate(2);
    } else {
        FldMenuCursorSetAt((u8)arg0, (f32)(fldMenuWork->topCursor << 6) + 32.0f, 12.0f);
    }
    trig = padPort0[0].trigger;
    if (trig & 0x40) {
        if (fldMenuWork->unk6 != 0) {
            fldMenuWork->unk6 = 0;
            FldMenuWinSetActive(0);
            SndSePlay(0x7D3, 0, 0, 0x1E, 0);
            return 0;
        }
        FldMenuClose();
        return 0xFF;
    }
    if (trig & 0x20) {
        switch (fldMenuWork->topCursor) {
        case 0:
            ret = 2;
            break;
        case 1:
            ret = 7;
            break;
        case 2:
            ret = 6;
            break;
        case 3:
            if (fldMenuWork->unk6 != 0) {
                ret = 8;
            } else {
                fldMenuWork->unk6 = 1;
            }
            break;
        case 4:
            ret = 5;
            break;
        case 5:
            ret = 3;
            break;
        }
        SndSePlay(0x7D2, 0, 0, 0x1E, 0);
    }
    return ret;
}

/* lq/sq quadword copy (as in src/main/431AF0.c), written as a macro so the
   address operands are not forced into temporaries first */
#define copyq(dst, src) __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

void VuBezier3(f32 *, FVECTOR *, f32);

void FldMenuCommandIconsUpdate(s32 arg0) {
    FVECTOR k[4];
    u8 id = arg0;
    FldMenuQuad *p;
    s32 i;
    s32 all;
    s32 x, y, z;
    s32 cx, cy;
    s32 px, py;

    FldMenuWinSetLayer(id, 0, &fldMenuTexIcons, fldMenuWork->topQuads, 128.0f);
    p = fldMenuWork->topQuads;
    if (fldMenuWork->screen == 0xFF) {
        FldMenuWinSetLayerVisible(id, 0, 1);
    }
    switch (fldMenuWork->state) {
    case 0:
        FldMenuWinSetLayerVisible(id, 0, 0);
        if (FldMenuWinGetState(id) >= 4) {
            fldMenuWork->state++;
            for (i = 0; i < 6; i++) {
                px = i * 64 + 32;
                py = 2;
                p->type = 1;
                p->z = 0;
                fldMenuWork->anims[i].timer = i * 4;
                p->v[0].r = 0x80;
                p->v[0].g = 0x80;
                p->v[0].b = 0x80;
                p->v[0].a = 0;
                p->v[0].x = px;
                p->v[0].y = py;
                p->v[1].x = px + 0x20;
                p->v[1].y = py + 0x20;
                p->v[0].u = fldMenuCommandIconUVs[i].u;
                p->v[0].v = fldMenuCommandIconUVs[i].v;
                p->v[1].u = fldMenuCommandIconUVs[i].u + 0x20;
                p->v[1].v = fldMenuCommandIconUVs[i].v + 0x20;
                fldMenuWork->anims[i].end[0] = px;
                fldMenuWork->anims[i].end[1] = py;
                fldMenuWork->anims[i].end[2] = 0.0f;
                fldMenuWork->anims[i].end[3] = 0.0f;
                copyq(&fldMenuWork->anims[i].start, &fldMenuWork->anims[i].end);
                fldMenuWork->anims[i].t = 0.0f;
                fldMenuWork->anims[i].start[0] -= 32.0f;
                p++;
            }
        }
        break;
    case 1:
        FldMenuWinSetLayerVisible(id, 0, 0);
        for (i = 0; i < 6; i++) {
            if (fldMenuWork->anims[i].timer == 0) {
                if ((s8)p->v[0].a >= 0) {
                    p->v[3].a = p->v[2].a = p->v[1].a = p->v[0].a = p->v[0].a + 8;
                }
                copyq(&k[0], &fldMenuWork->anims[i].start);
                copyq(&k[1], &fldMenuWork->anims[i].end);
                copyq(&k[2], &fldMenuWork->anims[i].end);
                copyq(&k[3], &fldMenuWork->anims[i].end);
                k[1][0] += 64.0f;
                fldMenuWork->anims[i].t += 0.1f;
                if (fldMenuWork->anims[i].t >= 1.0f) {
                    fldMenuWork->anims[i].t = 1.0f;
                }
                VuBezier3(fldMenuWork->anims[i].cur, k, fldMenuWork->anims[i].t);
                p->z = 0;
                cx = fldMenuWork->anims[i].cur[0] + 0.5f;
                cy = fldMenuWork->anims[i].cur[1] + 0.5f;
                p->v[0].y = cy;
                p->v[1].y = cy + 0x20;
                p->v[1].x = cx + 0x20;
                p->v[0].x = cx;
            } else {
                fldMenuWork->anims[i].timer--;
            }
            p++;
        }
        all = 1;
        for (i = 0; i < 6; i++) {
            if (fldMenuWork->anims[i].t != 1.0f) {
                all = 0;
            }
        }
        if (all == 1) {
            fldMenuWork->state++;
            for (i = 0; i < 6; i++) {
                copyq(&fldMenuWork->anims[i].start, &fldMenuWork->anims[i].end);
            }
        }
        break;
    case 2:
        FldMenuWinSetLayerVisible(id, 0, 1);
        for (i = 0; i < 6; i++) {
            fldMenuWork->anims[i].t += 0.03f;
            if (fldMenuWork->topCursor == i) {
                p->v[0].r = 0x80;
                p->v[0].g = 0x80;
                p->v[0].b = 0x80;
                p->v[0].a = 0x80;
                if (fldMenuWork->anims[i].t >= 1.0f) {
                    fldMenuWork->anims[i].t -= 1.0f;
                }
            } else {
                p->v[0].r = 0x40;
                p->v[0].g = 0x40;
                p->v[0].b = 0x40;
                p->v[0].a = 0x80;
                if (fldMenuWork->anims[i].t >= 1.0f) {
                    fldMenuWork->anims[i].t = 1.0f;
                }
            }
            copyq(&k[0], &fldMenuWork->anims[i].start);
            copyq(&k[1], &fldMenuWork->anims[i].start);
            copyq(&k[2], &fldMenuWork->anims[i].end);
            copyq(&k[3], &fldMenuWork->anims[i].end);
            k[0][2] = 0.0f;
            k[1][2] = 16.0f;
            k[2][2] = 16.0f;
            k[3][2] = 0.0f;
            k[1][1] -= 24.0f;
            k[2][1] -= 24.0f;
            VuBezier3(fldMenuWork->anims[i].cur, k, fldMenuWork->anims[i].t);
            p->z = 0;
            x = fldMenuWork->anims[i].cur[0] + 0.5f;
            z = fldMenuWork->anims[i].cur[2] + 0.5f;
            y = fldMenuWork->anims[i].cur[1] + 0.5f;
            x -= z;
            y -= z;
            p->v[1].x = x + (0x20 + z);
            p->v[1].y = y + (0x20 + z);
            p->v[0].x = x;
            p->v[0].y = y;
            p++;
        }
        break;
    }
    p->type = 0;
}

void FldMenuGuideUpdate(s32 arg0) {
    u8 tbl[3] = { 6, 4, 3 };
    u8 buf[4];
    s32 codes[32];
    FldMenuQuad *p;
    s32 n;
    s32 skip;
    s32 i;
    s32 j;
    s32 k;
    s32 x;
    s32 w;
    u8 h;
    s32 dy;
    s32 sy;
    s32 h2;

    func_0048281C(buf, 0, 4);
    if (fldMenuWork->unkB == fldMenuWork->guideMode && fldMenuWork->unkD == 3) {
        return;
    }
    p = fldMenuWork->topQuads2;
    sy = 24;
    fldMenuWork->unkC += 16;
    if (fldMenuWork->unkC >= tbl[fldMenuWork->unkD] * 20) {
        fldMenuWork->unkC = 0;
        fldMenuWork->unkD++;
    }
    if (fldMenuWork->unkD >= 3) {
        fldMenuWork->unkB = fldMenuWork->guideMode;
    }
    n = 0;
    skip = 0;
    switch (fldMenuWork->guideMode) {
    case 0:
    case 1:
        fldMenuWork->unk8 = 0xA000;
        n = 2;
        buf[0] = 0x21;
        skip = 0;
        buf[1] = 1;
        break;
    case 2:
        fldMenuWork->unk8 = 0x5000;
        buf[0] = 0x22;
        n = 3;
        buf[1] = 5;
        skip = 0;
        buf[2] = 0x20;
        break;
    case 3:
        fldMenuWork->unk8 = 0x5000;
        buf[0] = 0x23;
        n = 3;
        buf[1] = 4;
        skip = 0;
        buf[2] = 0x31;
        break;
    case 7:
        fldMenuWork->unk8 = 0x5000;
        n = 2;
        buf[0] = 0x23;
        skip = 0;
        buf[1] = 1;
        break;
    case 4:
        fldMenuWork->unk8 = 0xF000;
        n = 2;
        buf[0] = 0x23;
        skip = 0;
        buf[1] = 3;
        break;
    case 5:
        fldMenuWork->unk8 = 0x5000;
        n = 2;
        buf[1] = 1;
        skip = 1;
        break;
    case 6:
        fldMenuWork->unk8 = 0x5000;
        n = 2;
        skip = 0;
        buf[0] = 0x22;
        buf[1] = 0;
        break;
    case 8:
        fldMenuWork->unk8 = 0x5000;
        n = 3;
        buf[1] = 0x33;
        skip = 1;
        buf[2] = 0x20;
        break;
    case 9:
        fldMenuWork->unk8 = 0x5000;
        n = 2;
        buf[1] = 0x33;
        skip = 1;
        buf[2] = 0x20;
        break;
    case 10:
        fldMenuWork->unk8 = 0x5000;
        n = 2;
        buf[0] = 0x23;
        skip = 0;
        buf[1] = 1;
        break;
    case 11:
        fldMenuWork->unk8 = 0x5000;
        n = 2;
        buf[0] = 0x21;
        skip = 0;
        buf[1] = 1;
        break;
    case 12:
        fldMenuWork->unk8 = 0x5000;
        n = 2;
        buf[0] = 0x2D;
        skip = 0;
        buf[1] = 1;
        break;
    case 13:
        fldMenuWork->unk8 = 0x5000;
        n = 2;
        buf[0] = 0x2C;
        skip = 0;
        buf[1] = 1;
        break;
    case 14:
        fldMenuWork->unk8 = 0x5000;
        n = 2;
        buf[0] = 0x2E;
        skip = 0;
        buf[1] = 1;
        break;
    case 15:
        fldMenuWork->unk8 = 0x5000;
        n = 2;
        buf[0] = 0x2F;
        skip = 0;
        buf[1] = 1;
        break;
    case 16:
        fldMenuWork->unk8 = 0x5000;
        buf[0] = 0x30;
        n = 3;
        buf[1] = 1;
        skip = 0;
        buf[2] = 0x31;
        break;
    case 17:
        fldMenuWork->unk8 = 0x5000;
        buf[0] = 0x30;
        n = 3;
        buf[1] = 1;
        skip = 0;
        buf[2] = 0x32;
        break;
    case 18:
        fldMenuWork->unk8 = 0x5000;
        n = 2;
        buf[0] = 0x36;
        skip = 0;
        buf[1] = 1;
        break;
    }
    if (!skip) {
        p->type = 1;
        p->v[0].r = 0x80;
        p->v[0].g = 0x80;
        p->v[0].b = 0x80;
        p->v[0].a = 0x80;
        p->v[0].x = 0x80;
        p->v[1].x = 0x98;
        p->v[1].y = 0x30;
        p->v[0].u = 0x56;
        p->v[0].v = 0xBE;
        p->v[1].u = 0x76;
        p->v[1].v = 0xDE;
        p->v[0].y = sy;
        p->z = 0;
        p++;
    }
    if (n == 3 && fldMenuWork->unkD >= 2) {
        p->type = 1;
        p->v[0].r = 0x80;
        p->v[0].g = 0x80;
        p->v[0].b = 0x80;
        p->v[0].a = 0x80;
        p->v[0].x = 0x1CA;
        p->v[0].y = sy;
        p->v[1].x = 0x1E2;
        p->v[1].y = 0x30;
        p->v[0].u = 0x76;
        p->v[0].v = 0xE0;
        p->v[1].u = 0x96;
        p->v[1].v = 0x100;
        p->z = 0;
        p++;
    }
    p->type = 0;
    {
        s8 offs[5][2] = { { 2, 2 }, { -2, -2 }, { 2, -2 }, { -2, 2 }, { 0, 0 } };
        u16 base[3] = { 0x9E, 0x150, 0x1E8 };
        s8 c0;
        s8 c1;
        u8 cnt;
        f32 k125;

        p = fldMenuWork->topQuads3;
        k125 = 1.25f;
        for (i = skip; i < n; i++) {
            x = 0;
            /* cast: TxtMsgGetLength's u8 return masks cnt differently */
            cnt = (u8)((s32 (*)(s32, s32))TxtMsgGetLength)(sysGrxsysPac->msg->tbl[0], buf[i]);
            TxtGlyphQueueMsg(5, sysGrxsysPac->msg->tbl[0], i + 1, buf[i]);
            TxtMsgGetCodes((u8 *)sysGrxsysPac->msg->tbl[0], buf[i], codes);
            for (j = 0; j < cnt; j++) {
                TxtGlyphGetMetrics(codes[j], (u8 *)&c0, (u8 *)&c1, sysGrxsysPac->glyphTable);
                w = (f32)(c0 - c1) * k125;
                if (i >= fldMenuWork->unkD) {
                    if (fldMenuWork->unkD == i) {
                        w = (f32)w * fldMenuWork->unkC / (tbl[i] * 20.0f);
                        h = fldMenuWork->unkC / tbl[i];
                    } else {
                        h = 0;
                    }
                } else {
                    h = 20;
                }
                dy = (h >> 1) - 10;
                h2 = 25;
                for (k = 0; k < 5; k++) {
                    p->type = 1;
                    if (k == 4) {
                        p->v[0].r = 0x80;
                        p->v[0].g = 0x80;
                        p->v[0].b = 0x80;
                        p->v[0].a = 0x80;
                    } else {
                        p->v[0].r = 0x10;
                        p->v[0].g = 0x20;
                        p->v[0].b = 0x40;
                        p->v[0].a = 0x60;
                    }
                    p->v[0].x = base[i] + x + offs[k][0];
                    p->v[0].y = offs[k][1] + h2 - dy;
                    p->v[1].x = x + w + (offs[k][0] + base[i]);
                    p->v[1].y = offs[k][1] + h2 - dy + h;
                    p->z = 0;
                    p->v[0].u = c1 + j * 18;
                    p->v[0].v = i * 18 + 18;
                    p->v[1].u = c1 + j * 18 + (c0 - c1);
                    p->v[1].v = i * 18 + 34;
                    p++;
                }
                x += w + 2;
            }
        }
    }
    p->type = 0;
}
