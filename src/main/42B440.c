/* rodata: 0x004A5770-0x004A5948 */
#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "pad.h"
#include "main/n02.h"

typedef struct {
    s32 w;
    s32 h;
    u8 pad8[8];
    u32 type;
    s32 x14;
    u8 pad18[0x20];
    s32 x38;
    u8 pad3C[0x24];
    s32 x60;
    u8 pad64[4];
    s32 x68;
    s32 x6C;
    u32 x70;
    s32 x74;
} Obj;

typedef struct {
    u8 pad0[8];
    Obj *obj;
    u8 padC[0x2C0 - 0xC];
} Entry;

typedef struct {
    u32 unk0;
    u32 count;
    u8 pad8[0x38];
    Entry entries[1];
} Hdr;

extern void GsTexUpload(void *);
extern u32 dbgTexNo;
extern s32 dbgTexLine;
extern s32 dbgTexCursor;
extern u8 dbgTexStrTextureInfo[];
extern u8 dbgTexStrTexNo[];
extern u8 dbgTexStrId[];
extern u8 dbgTexStrWidth[];
extern u8 dbgTexStrHeight[];
extern u8 dbgTexStrFormat16[];
extern u8 dbgTexStrFormat256[];
extern u8 dbgTexStrFormatFull[];
extern u8 dbgTexStrFormat24bit[];
extern u8 dbgTexStrFormat16bit[];
extern u8 dbgTexStrUnSurpport[];
extern u8 dbgTexStrMipmap[];
extern u8 dbgTexStrClut[];
extern u8 dbgTexStrLineraT[];
extern u8 dbgTexStrLineraB[];
extern u8 dbgTexStrSpecularOn[];
extern u8 dbgTexStrSpecularOff[];
extern u8 dbgTexStrMipmapL[];
extern u8 dbgTexStrMipmapK[];
extern u8 dbgTexStrAllMipmapL[];
extern u8 dbgTexStrAllMipmapK[];
extern u8 dbgTexStrAllLineraT[];
extern u8 dbgTexStrAllLineraB[];

void DbgTexturePage(void) {
    u8 buf[0x140];
    GphBatchQuad prims[8];
    Pad *pad;
    Hdr *hdr;
    Entry *ent;
    Obj *obj;
    GphBatchQuad *p;
    u32 i;
    s32 v;
    u32 c;
    s32 w;
    s32 a;

    p = prims;
    pad = &padPort1;
    hdr = GphTexTableGetCurrent();
    if (hdr != NULL) {
        ent = hdr->entries;
        obj = ent[dbgTexNo].obj;
        if (!(pad->held & 2)) {
            dbgTexLine = DbgMenuLineSelect(dbgTexLine, 8);
            switch (dbgTexLine) {
            case 0:
                dbgTexCursor = dbgTexLine;
                if (pad->repeatFast & 0x8000) {
                    c = dbgTexNo;
                    if (c == 0) {
                        c = hdr->count;
                    }
                    dbgTexNo = c - 1;
                }
                if (pad->repeatFast & 0x2000) {
                    if (dbgTexNo < hdr->count - 1) {
                        dbgTexNo++;
                    } else {
                        dbgTexNo = 0;
                    }
                }
                break;
            case 1:
                dbgTexCursor = 4;
                if (obj != NULL) {
                    if (pad->repeatFast & 0x8000) {
                        if (obj->x14 != 0) {
                            obj->x14--;
                        }
                    }
                    if (pad->repeatFast & 0x2000) {
                        obj->x14++;
                    }
                }
                break;
            case 2:
                dbgTexCursor = 6;
                if (obj != NULL) {
                    if (pad->repeatFast & 0x8000) {
                        obj->x68 = 0;
                    }
                    if (pad->repeatFast & 0x2000) {
                        obj->x68 = 1;
                    }
                }
                break;
            case 3:
                dbgTexCursor = 7;
                if (obj != NULL) {
                    if (pad->repeatFast & 0x8000) {
                        obj->x6C = 0;
                    }
                    if (pad->repeatFast & 0x2000) {
                        obj->x6C = 1;
                    }
                }
                break;
            case 4:
                dbgTexCursor = 8;
                if (obj != NULL) {
                    if (pad->repeatFast & 0x8000) {
                        if (obj->x70 != 0) {
                            obj->x70--;
                        }
                    }
                    if (pad->repeatFast & 0x2000) {
                        if (obj->x70 < 3) {
                            obj->x70++;
                        }
                    }
                }
                break;
            case 5:
                dbgTexCursor = 9;
                if (obj != NULL) {
                    if (pad->repeatFast & 0x8000) {
                        obj->x74--;
                    }
                    if (pad->repeatFast & 0x2000) {
                        obj->x74++;
                    }
                    obj->x74 &= 0xFFF;
                }
                break;
            case 6:
                dbgTexCursor = 10;
                if (obj != NULL) {
                    if (pad->repeatFast & 0x8000) {
                        if (obj->x70 != 0) {
                            obj->x70--;
                        }
                        v = obj->x70;
                        for (i = 0; i < hdr->count; i++) {
                            obj = ent[i].obj;
                            if (obj != NULL) {
                                obj->x70 = v;
                            }
                        }
                        obj = ent[dbgTexNo].obj;
                    }
                    if (pad->repeatFast & 0x2000) {
                        if (obj->x70 < 3) {
                            obj->x70++;
                        }
                        v = obj->x70;
                        for (i = 0; i < hdr->count; i++) {
                            obj = ent[i].obj;
                            if (obj != NULL) {
                                obj->x70 = v;
                            }
                        }
                    }
                }
                break;
            case 7:
                dbgTexCursor = 11;
                if (obj != NULL) {
                    if (pad->repeatFast & 0x8000) {
                        obj->x74 = (obj->x74 - 1) & 0xFFF;
                        w = obj->x74;
                        for (i = 0; i < hdr->count; i++) {
                            obj = ent[i].obj;
                            if (obj != NULL) {
                                obj->x74 = w;
                            }
                        }
                        obj = ent[dbgTexNo].obj;
                    }
                    if (pad->repeatFast & 0x2000) {
                        obj->x74 = (obj->x74 + 1) & 0xFFF;
                        w = obj->x74;
                        for (i = 0; i < hdr->count; i++) {
                            obj = ent[i].obj;
                            if (obj != NULL) {
                                obj->x74 = w;
                            }
                        }
                    }
                }
                break;
            case 8:
                dbgTexCursor = 12;
                if (obj != NULL) {
                    if (pad->repeatFast & 0x8000) {
                        obj->x68 = 0;
                        for (i = 0; i < hdr->count; i++) {
                            obj = ent[i].obj;
                            if (obj != NULL) {
                                obj->x68 = 0;
                            }
                        }
                        obj = ent[dbgTexNo].obj;
                    }
                    if (pad->repeatFast & 0x2000) {
                        obj->x68 = 1;
                        v = obj->x68;
                        for (i = 0; i < hdr->count; i++) {
                            obj = ent[i].obj;
                            if (obj != NULL) {
                                obj->x68 = v;
                            }
                        }
                    }
                }
                break;
            }
        }
        DbgPrintf(3, 3, dbgTexStrTextureInfo);
        DbgMenuCursorDraw(2, dbgTexCursor + 6);
        DbgPrintf(3, 5, dbgTexStrTexNo, dbgTexNo, hdr->count);
        obj = ent[dbgTexNo].obj;
        if (obj != NULL) {
            if (!(pad->held & 2)) {
                DbgPrintf(3, 6, dbgTexStrId, obj->x60);
                DbgPrintf(3, 7, dbgTexStrWidth, obj->w);
                DbgPrintf(3, 8, dbgTexStrHeight, obj->h);
                switch (obj->type) {
                case 20:
                    DbgPrintf(3, 9, dbgTexStrFormat16);
                    break;
                case 19:
                    DbgPrintf(3, 9, dbgTexStrFormat256);
                    break;
                case 0:
                    DbgPrintf(3, 9, dbgTexStrFormatFull);
                    break;
                case 1:
                    DbgPrintf(3, 9, dbgTexStrFormat24bit);
                    break;
                case 2:
                    DbgPrintf(3, 9, dbgTexStrFormat16bit);
                    break;
                default:
                    DbgPrintf(3, 9, dbgTexStrUnSurpport);
                    break;
                }
                DbgPrintf(3, 10, dbgTexStrMipmap, obj->x14);
                DbgPrintf(3, 11, dbgTexStrClut, obj->x38);
                if (obj->x68 == 1) {
                    DbgPrintf(3, 12, dbgTexStrLineraT);
                } else {
                    DbgPrintf(3, 12, dbgTexStrLineraB);
                }
                if (obj->x6C == 1) {
                    DbgPrintf(3, 13, dbgTexStrSpecularOn);
                } else {
                    DbgPrintf(3, 13, dbgTexStrSpecularOff);
                }
                DbgPrintf(3, 14, dbgTexStrMipmapL, obj->x70);
                a = (s16)obj->x74;
                if (a & 0x800) {
                    a |= ~0xFFF;
                }
                DbgPrintf(3, 15, dbgTexStrMipmapK, (f32)a * 0.0625f);
                DbgPrintf(3, 16, dbgTexStrAllMipmapL, obj->x70);
                a = (s16)obj->x74;
                if (a & 0x800) {
                    a |= ~0xFFF;
                }
                DbgPrintf(3, 17, dbgTexStrAllMipmapK, (f32)a * 0.0625f);
                if (obj->x68 == 1) {
                    DbgPrintf(3, 18, dbgTexStrAllLineraT);
                } else {
                    DbgPrintf(3, 18, dbgTexStrAllLineraB);
                }
            }
            GsTexLoadPacket(buf, obj, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
            GsTexUpload(buf);
            GsTexPacketSend(buf);
            p->type = 1;
            p->v[0].r = 0x80;
            p->v[0].g = 0x80;
            p->v[0].b = 0x80;
            p->v[0].a = 0x80;
            p->v[1].r = 0x80;
            p->v[1].g = 0x80;
            p->v[1].b = 0x80;
            p->v[1].a = 0x80;
            p->v[2].r = 0x80;
            p->v[2].g = 0x80;
            p->v[2].b = 0x80;
            p->v[2].a = 0x80;
            p->v[3].r = 0x80;
            p->v[3].g = 0x80;
            p->v[3].b = 0x80;
            p->v[3].a = 0x80;
            p->v[0].u = 0;
            p->v[0].v = 0;
            p->v[1].u = 0;
            p->v[1].v = obj->h;
            p->v[2].u = obj->w;
            p->v[2].v = 0;
            p->v[3].u = obj->w;
            p->v[3].v = obj->h;
            p->v[0].x = 0x260 - obj->w;
            p->v[0].y = 0x190 - obj->h;
            p->v[1].x = 0x260 - obj->w;
            p->v[1].y = 0x190;
            p->v[2].x = 0x260;
            p->v[2].y = 0x190 - obj->h;
            p->v[3].x = 0x260;
            p->v[3].y = 0x190;
            p->z = 0;
            p++;
            p->type = 0;
            GphBatchDrawQuads(prims);
        }
    }
}

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 trig;
    u16 unk6;
    u16 held;
} Pad2;

s32 DbgMemDumpPage(s32 base, Pad *pad) {
    char bits[5][4] = { "128", " 64", " 32", " 16", "  8" };
    u8 order[5][16] = {
        { 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 },
        { 7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8 },
        { 3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12 },
        { 1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14 },
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
    };
    s32 a;
    s32 addr;
    u32 *p32;
    u8 *p8;
    u32 i;
    s32 r;

    p32 = (u32 *)(base + dbgMemView.ofs * 8);
    addr = 0x1000000;
    if ((s32)p32 > 0xFFFFF) {
        addr = (s32)p32;
    }
    p32 = (u32 *)addr;
    p8 = (u8 *)addr;
    DbgPrintf(4, 4, "MEMORY VIEW & EDIT %08x [%s bit]", p8, bits[dbgMemView.mode]);
    if (pad->trigger & 2) {
        dbgMemView.mode++;
        if (dbgMemView.mode >= 5) {
            dbgMemView.mode = 0;
        }
    }
    if (dbgMemView.edit == 0) {
        if (pad->trigger & 0x20) {
            dbgMemView.edit = 1;
        }
        if (pad->repeatFast & 1) {
            if (pad->repeatFast & 0x1000) {
                dbgMemView.ofs -= 8;
            }
            if (pad->repeatFast & 0x4000) {
                dbgMemView.ofs += 8;
            }
        } else {
            if (pad->repeatFast & 0x1000) {
                dbgMemView.ofs -= 2;
            }
            if (pad->repeatFast & 0x4000) {
                dbgMemView.ofs += 2;
            }
        }
        if (pad->repeatFast & 1) {
            if (pad->repeatFast & 0x8000) {
                dbgMemView.ofs -= 0x8000;
            }
            if (pad->repeatFast & 0x2000) {
                dbgMemView.ofs += 0x8000;
            }
        } else {
            if (pad->repeatFast & 0x8000) {
                dbgMemView.ofs -= 0x1000;
            }
            if (pad->repeatFast & 0x2000) {
                dbgMemView.ofs += 0x1000;
            }
        }
    } else {
        if ((pad->repeatFast & 0x8000) && dbgMemView.col != 0) {
            dbgMemView.col -= 2;
        }
        if ((pad->repeatFast & 0x2000) && dbgMemView.col < 30) {
            dbgMemView.col += 2;
        }
        if (pad->repeatFast & 0x1000) {
            if (dbgMemView.row != 0) {
                dbgMemView.row--;
            } else if (dbgMemView.ofs != 0) {
                dbgMemView.ofs--;
            }
        }
        if (pad->repeatFast & 0x4000) {
            if (dbgMemView.row < 31) {
                dbgMemView.row++;
            } else {
                dbgMemView.ofs++;
            }
        }
        if (pad->repeatFast & 0x20) {
            ((u8 *)addr)[dbgMemView.row * 16 + order[dbgMemView.mode][dbgMemView.col / 2]]++;
        }
        if (pad->repeatFast & 0x40) {
            ((u8 *)addr)[dbgMemView.row * 16 + order[dbgMemView.mode][dbgMemView.col / 2]]--;
        }
    }
    for (i = 0; i < 14; i++) {
        switch (dbgMemView.mode) {
        case 0:
            DbgPrintf(2, i + 6, "%08X:%08X-%08X-%08X-%08X ", p32, p32[3], p32[2], p32[1], p32[0]);
            p32 += 4;
            break;
        case 1:
            DbgPrintf(2, i + 6, "%08X:%08X-%08X-%08X-%08X ", p32, p32[1], p32[0], p32[3], p32[2]);
            p32 += 4;
            break;
        case 2:
            DbgPrintf(2, i + 6, "%08X:%08X-%08X-%08X-%08X ", p32, p32[0], p32[1], p32[2], p32[3]);
            p32 += 4;
            break;
        case 3:
            DbgPrintf(2, i + 6, "%08X:%04X%04X-%04X%04X-%04X%04X-%04X%04X ", ((u16 *)addr),
                          ((u16 *)addr)[0], ((u16 *)addr)[1], ((u16 *)addr)[2], ((u16 *)addr)[3], ((u16 *)addr)[4], ((u16 *)addr)[5], ((u16 *)addr)[6], ((u16 *)addr)[7]);
            addr += 16;
            break;
        case 4:
            DbgPrintf(2, i + 6, "%08X:%02X%02X%02X%02X-%02X%02X%02X%02X-%02X%02X%02X%02X-%02X%02X%02X%02X ", p8,
                          p8[0], p8[1], p8[2], p8[3], p8[4], p8[5], p8[6], p8[7],
                          p8[8], p8[9], p8[10], p8[11], p8[12], p8[13], p8[14], p8[15]);
            p8 += 16;
            break;
        }
    }
    /* one local for &dbgMemView and its col: col lands in the pointer's register */
    a = (s32)&dbgMemView;
    r = ((DbgMemView *)a)->edit;
    if (r != 0) {
        a = ((DbgMemView *)a)->col;
        r = a < 24;
    }
    return r;
}
