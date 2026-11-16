#include "common.h"
#include "protos/field.h"
#include "field/n21.h"

typedef struct {
    s16 u0;
    s16 x;
    s16 z;
    s16 rot;
    s16 v[6];
} Pos;

extern Pos amapGimmickPos;

void AmapGimmickBoxGet(s32, Pos *);
void AmapAreaAdd(AmapWork *, s32, s32, s32, s32, s32, s32, s32, s32);
s32 AmapGetExitDest(AmapWork *);

#define ENT_SET(t)                \
    ents[p->objCount].type = (t);       \
    ents[p->objCount].x = x;            \
    ents[p->objCount].y = 0;            \
    ents[p->objCount].z = z;

/* Most AmapOccAdd calls go through a cast to (AmapWork *, s32, s32, s32, s32):
 * through the real s16 parameter the u16 area count is loaded signed (lh, not
 * lhu) and the code differs (round A dropped every cast that wasn't needed). */
void AmapGimmicksSpawn(AmapWork *p) {
    AmapPlace *srcs;
    AmapOccsRec *curs;
    AmapOccsRec *cur;
    AmapObjRec *ents;
    AmapPlace *e;
    s32 i;
    s32 j;
    s32 jj;
    s32 w4;
    s32 x;
    s32 z;
    s32 k;
    s32 flag;
    s32 type;
    s32 bit20;
    s32 fp;
    s32 idx;
    s32 r;
    s32 y;
    s32 y2;
    s32 kind2;
    s32 kind;
    s32 val;
    s32 t4;
    s32 d2;
    s32 e0;
    s32 e1;
    s32 e2;
    s32 e3;
    s16 *pa;
    s16 *pb;
    s32 v;

    idx = 0;
    srcs = p->gimmicks;
    curs = p->occs;
    p->occCount = 0;
    ents = p->objs;
    for (i = 0; i < 64; i++) {
        cur = &curs[p->occsCount];
        cur->id = 0;
        cur->areaId = 0;
        cur->flags = 0;
        for (j = 6; j >= 0; j--) {
            cur->cond[j] = 0;
        }
        for (j = 31; j >= 0; j--) {
            cur->ops[j] = 0;
        }
        for (j = 31; j >= 0; j--) {
            cur->args[j] = 0;
        }
    }

    for (i = 0; i < p->gimmickCount; i++) {
        flag = 0;
        e = &srcs[i];
        k = 0;
        cur = &curs[p->occsCount];
        type = e->type & 0xDF;
        bit20 = e->type & 0x20;
        w4 = e->arg;
        fp = type & 0x1F;
        x = e->x * 500 - 250;
        z = e->y * 500 - 250;
        amapGimmickPos.rot = (u32)type >> 6;
        amapGimmickPos.z = z;
        amapGimmickPos.x = x;
        switch (fp) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 24:
        case 25:
        case 29:
        case 30:
            flag = 1;
            break;
        }
        if (flag == 0) {
            if (fp == 0x1C) {
                continue;
            }
            ENT_SET(type);
            switch (p->mapType) {
            default:
            case 0:
                switch (type & 0x1F) {
                case 14:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x170, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                case 15:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x178, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                }
                break;
            case 1:
                switch (type & 0x1F) {
                case 13:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x268, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                case 17:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x288, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                case 18:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x290, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                case 26:
                case 27:
                    idx = p->objCount;
                    y = VuRandInt(15) * 50;
                    ents[p->objCount].y = y;
                    AmapOccAddFloat(p, 0xAA, -1, idx, x, y + 20, z);
                    AmapOccAddFloat(p, 0xAB, -1, idx, 0, 0, 0);
                    break;
                }
                break;
            case 2:
                switch (type & 0x1F) {
                case 26:
                case 27:
                    idx = p->objCount;
                    y2 = VuRandInt(15) * 50;
                    ents[p->objCount].y = y2 - 200;
                    AmapOccAddFloat(p, 0xAA, -1, idx, x, y2, z);
                    AmapOccAddFloat(p, 0xAB, -1, idx, 0, 0, 0);
                    break;
                }
                break;
            case 3:
                switch (type & 0x1F) {
                case 14:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x470, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                case 15:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x478, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                case 20:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x4A0, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                }
                break;
            case 4:
                switch (type & 0x1F) {
                case 14:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x570, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                case 15:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x578, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                case 18:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x590, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                }
                break;
            case 5:
                switch (type & 0x1F) {
                case 21:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x6A8, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                case 22:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x6B0, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                }
                break;
            case 6:
                switch (type & 0x1F) {
                case 18:
                    idx = p->objCount;
                    AmapGimmickBoxGet(0x790, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                    break;
                }
                break;
            case 7:
                break;
            }
            p->objCount++;
        }
        switch (type & 0x1F) {
        case 0:
            ENT_SET(type);
            idx = p->objCount; p->objCount = idx + 1;
            switch (p->mapType) {
            default:
            case 0:
                t4 = p->areaCount;
                AmapGimmickBoxGet(0x101, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                if (bit20) {
                    AmapOccAdd(p, 0x1A, -1, idx, 1);
                    AmapOccAdd(p, 0x5A, -1, idx, 0);
                    AmapOccAdd(p, 0x5B, -1, idx, 0);
                    AmapOccAdd(p, 0x5C, -1, idx, 3);
                    AmapOccAdd(p, 0xFF, -1, idx, 0xDB4);
                    AmapOccAdd(p, 0x5D, -1, idx, 3);
                    AmapOccAdd(p, 0x5E, -1, idx, 3);
                    AmapOccAdd(p, 0x5F, -1, idx, 3);
                    AmapOccAdd(p, 0x60, -1, idx, 3);
                    AmapOccAdd(p, 0x61, -1, 0, 0);
                    AmapOccAdd(p, 0x62, -1, 0, 0);
                    AmapOccAdd(p, 0x63, -1, t4, 0);
                } else {
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x50, p->areaCount, 0, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xFF, p->areaCount, 0, 0xDB7);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x66, p->areaCount, idx, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x51, p->areaCount, idx, 1);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xFF, p->areaCount, idx, 0xDB4);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x52, p->areaCount, idx, 1);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x53, p->areaCount, idx, 1);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x54, p->areaCount, idx, 0);
                    AmapGimmickBoxGet(0x102, &amapGimmickPos);
                    AmapAreaAddBox(p, 2, idx, &amapGimmickPos);
                }
                AmapGimmickBoxGet(0x100, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                break;
            case 1:
                if (bit20) {
                    AmapOccAdd(p, 0x1A, -1, idx, 1);
                    AmapOccAdd(p, 0x5A, -1, idx, 0);
                    AmapOccAdd(p, 0x5B, -1, idx, 0);
                    AmapOccAdd(p, 0xA5, -1, idx, 0);
                    AmapOccAdd(p, 0x61, -1, 0, 0);
                    AmapOccAdd(p, 0x62, -1, 0, 0);
                } else {
                    AmapOccAdd(p, 0xA7, -1, idx, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x50, p->areaCount, 0, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xFF, p->areaCount, 0, 0xDB7);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x66, p->areaCount, idx, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x51, p->areaCount, idx, 1);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x54, p->areaCount, idx, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xA6, p->areaCount, idx, 0);
                    AmapGimmickBoxGet(0x202, &amapGimmickPos);
                    AmapAreaAddBox(p, 2, idx, &amapGimmickPos);
                }
                AmapGimmickBoxGet(0x200, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                break;
            case 2:
                if (bit20) {
                    AmapOccAdd(p, 0x1A, -1, idx, 1);
                    AmapOccAdd(p, 0x5A, -1, idx, 0);
                    AmapOccAdd(p, 0x5B, -1, idx, 0);
                    AmapOccAdd(p, 0xA5, -1, idx, 0);
                    AmapOccAdd(p, 0x61, -1, 0, 0);
                    AmapOccAdd(p, 0x62, -1, 0, 0);
                } else {
                    AmapOccAdd(p, 0xA7, -1, idx, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x50, p->areaCount, 0, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xFF, p->areaCount, 0, 0xDB7);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x66, p->areaCount, idx, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x51, p->areaCount, idx, 1);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x54, p->areaCount, idx, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xA6, p->areaCount, idx, 0);
                    AmapGimmickBoxGet(0x202, &amapGimmickPos);
                    AmapAreaAddBox(p, 2, idx, &amapGimmickPos);
                }
                AmapGimmickBoxGet(0x200, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                AmapGimmickBoxGet(0x301, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                if (bit20) {
                    AmapOccAdd(p, 0x1A, -1, idx, 1);
                    AmapOccAdd(p, 0x5A, -1, idx, 0);
                    AmapOccAdd(p, 0x5B, -1, idx, 0);
                    AmapOccAdd(p, 0xA0, -1, idx, 0);
                    AmapOccAdd(p, 0x61, -1, 0, 0);
                    AmapOccAdd(p, 0x62, -1, 0, 0);
                } else {
                    AmapOccAdd(p, 0xA2, -1, idx, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x50, p->areaCount, 0, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xFF, p->areaCount, 0, 0xDB7);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x66, p->areaCount, idx, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x51, p->areaCount, idx, 1);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x54, p->areaCount, idx, 0);
                    ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xA1, p->areaCount, idx, 0);
                    AmapGimmickBoxGet(0x302, &amapGimmickPos);
                    AmapAreaAddBox(p, 2, idx, &amapGimmickPos);
                }
                AmapGimmickBoxGet(0x300, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                break;
            }
            break;
        case 1:
            if (p->mapType != 2) {
                ENT_SET(type);
                idx = p->objCount; p->objCount = idx + 1;
                switch (p->mapType) {
                case 0:
                default:
                    AmapOccAdd(p, 0x96, -1, p->areaCount, 0);
                    AmapOccAdd(p, 0x97, -1, p->areaCount, 0);
                    AmapGimmickBoxGet(0x108, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, -1, &amapGimmickPos);
                    break;
                case 1:
                    AmapOccAdd(p, 0x96, -1, p->areaCount, 0);
                    AmapOccAdd(p, 0x97, -1, p->areaCount, 0);
                    AmapGimmickBoxGet(0x208, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, -1, &amapGimmickPos);
                    break;
                case 3:
                    AmapOccAdd(p, 0x96, -1, p->areaCount, 0);
                    AmapOccAdd(p, 0x97, -1, p->areaCount, 0);
                    AmapGimmickBoxGet(0x308, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, -1, &amapGimmickPos);
                    break;
                }
            }
            break;
        case 2:
            if (p->mapType != 2) {
                ENT_SET(type);
                idx = p->objCount; p->objCount = idx + 1;
            }
            r = AmapGetExitDest(p);
            type = p->areaCount;
            switch (p->mapType) {
            default:
            case 0:
                AmapGimmickBoxGet(0x110, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                break;
            case 1:
                AmapGimmickBoxGet(0x210, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                break;
            case 2:
                break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                AmapGimmickBoxGet(0x310, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                break;
            }
            if (r < 0) {
                kind2 = 1;
                val = 0;
                t4 = 0;
            } else {
                t4 = p->floor - 1;
                if (r & 0x40000000) {
                    val = r;
                    kind2 = 2;
                } else {
                    kind2 = 3;
                    val = 0;
                }
            }
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, kind2, p->areaCount, idx, val);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x98, p->areaCount, type, 0);
            AmapGimmickBoxGet(0x11, &amapGimmickPos);
            AmapAreaAddBox(p, 2, idx, &amapGimmickPos);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 4, p->areaCount, -1, t4);
            AmapGimmickBoxGet(0x10, &amapGimmickPos);
            AmapAreaAddBox(p, 3, idx, &amapGimmickPos);
            break;
        case 3:
            ENT_SET(type);
            idx = p->objCount; p->objCount = idx + 1;
            if (p->mapType == 2) {
                AmapGimmickBoxGet(0x219, &amapGimmickPos);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xB4, p->areaCount, idx, 0);
                t4 = p->areaCount;
                AmapAreaAddBox(p, 1, -1, &amapGimmickPos);
                AmapGimmickBoxGet(0x218, &amapGimmickPos);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xB5, t4, p->areaCount, 0);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                break;
            }
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xFF, p->areaCount, -1, 0xDB2);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x32, p->areaCount, idx, 1);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x32, p->areaCount, idx, 2);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x33, p->areaCount, idx, 0);
            if (p->mapType != 6) {
                AmapGimmickBoxGet(0x19, &amapGimmickPos);
                AmapAreaAddBox(p, 5, -1, &amapGimmickPos);
                AmapGimmickBoxGet(0x18, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
            } else {
                AmapGimmickBoxGet(0x719, &amapGimmickPos);
                AmapAreaAddBox(p, 5, -1, &amapGimmickPos);
                AmapGimmickBoxGet(0x718, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
            }
            if (p->mapType == 5) {
                ENT_SET(type | 0x1F);
                p->objCount++;
            }
            break;
        case 4:
            ENT_SET(type);
            idx = p->objCount;
            p->unk22 = idx;
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x3C, p->areaCount, p->areaCount, 4);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x3D, p->areaCount, p->areaCount, 4);
            AmapOccAdd(p, 0x3E, -1, idx, 4);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xFF, p->areaCount, idx, 0xDB2);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x3F, p->areaCount, idx, 5);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x40, p->areaCount, idx, 0);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x41, p->areaCount, p->areaCount, 0);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x42, p->areaCount, 5, 0);
            if (p->mapType != 6) {
                AmapGimmickBoxGet(0x21, &amapGimmickPos);
                AmapAreaAddBox(p, 5, -1, &amapGimmickPos);
                AmapGimmickBoxGet(0x20, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
            } else {
                AmapGimmickBoxGet(0x721, &amapGimmickPos);
                AmapAreaAddBox(p, 5, -1, &amapGimmickPos);
                AmapGimmickBoxGet(0x720, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
            }
            p->objCount++;
            if (p->mapType == 5) {
                ENT_SET(type | 0x1F);
                p->objCount++;
            }
            break;
        case 24:
            ENT_SET(type);
            idx = p->objCount; p->objCount = idx + 1;
            p->groups[0].gates[p->groups[0].gateCount].id = idx;
            p->groups[0].gates[p->groups[0].gateCount].key = w4;
            p->groups[0].gates[p->groups[0].gateCount].area = (u8)p->areaCount;
            p->groups[0].gateCount++;
            if (p->mapType != 6) {
                AmapGimmickBoxGet(0xC1, &amapGimmickPos);
                AmapAreaAddBox(p, 4, -1, &amapGimmickPos);
                AmapGimmickBoxGet(0xC0, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
            } else {
                AmapGimmickBoxGet(0x7C2, &amapGimmickPos);
                AmapAreaAddBox(p, 4, -1, &amapGimmickPos);
                AmapGimmickBoxGet(0x7C1, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
            }
            AmapOccAdd(p, 0x14, -1, idx, 0);
            if (p->mapType == 5) {
                ENT_SET(type | 0x1F);
                p->objCount++;
            }
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            ENT_SET(type);
            idx = p->objCount;
            switch (p->mapType) {
            case 0:
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x46, p->areaCount, idx, 0);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x47, p->areaCount, idx, 0);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x48, p->areaCount, p->unk22, (type & 0x3F) - 5);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x49, p->areaCount, p->unk22, (type & 0x3F) - 5);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x67, p->areaCount, idx, 0);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 8, p->areaCount, idx, (type & 0x3F) - 5);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xFF, p->areaCount, idx, 0xDB0);
                AmapGimmickBoxGet(0x29, &amapGimmickPos);
                AmapAreaAddBox(p, 4, -1, &amapGimmickPos);
                AmapGimmickBoxGet(0x28, &amapGimmickPos);
                AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                p->objCount++;
                break;
            case 1:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                AmapOccAdd(p, 0xAA, -1, idx, 1);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xAB, p->areaCount, idx, 1);
                AmapOccAdd(p, 0xAC, -1, p->unk22, (type & 0x3F) - 5);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xAD, p->areaCount, p->unk22, (type & 0x3F) - 5);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x67, p->areaCount, idx, 0);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 8, p->areaCount, idx, (type & 0x3F) - 5);
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xFF, p->areaCount, idx, 0xDB1);
                AmapGimmickBoxGet(0x29, &amapGimmickPos);
                AmapAreaAddBox(p, 4, -1, &amapGimmickPos);
                if (p->mapType != 1 && p->mapType != 2) {
                    AmapGimmickBoxGet(0x28, &amapGimmickPos);
                    AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
                }
                p->objCount++;
                break;
            }
            break;
        case 9:
        case 10:
            pa = cur->ops;
            pb = cur->args;
            cur->id = p->occsCount;
            ENT_SET(type);
            idx = p->objCount;
            AmapGimmickBoxGet(0x48, &amapGimmickPos);
            AmapAreaAddBox(p, 0, idx, &amapGimmickPos);
            kind = 0xA;
            if (fp == 10) kind = 0xB;
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, kind, p->areaCount, idx, 0);
            type = p->areaCount;
            AmapGimmickBoxGet(0x4A, &amapGimmickPos);
            AmapAreaAddBox(p, 1, -1, &amapGimmickPos);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, kind, p->areaCount, idx, 0);
            d2 = p->areaCount;
            AmapGimmickBoxGet(0x4B, &amapGimmickPos);
            AmapAreaAddBox(p, 1, -1, &amapGimmickPos);
            AmapOccAdd(p, 0x82, -1, type, 0);
            AmapOccAdd(p, 0x82, -1, d2, 0);
            /* occCount is read as s16 here (u16 elsewhere) */
            e0 = (s16)p->occCount;
            AmapOccAdd(p, 0x83, -1, type, 1);
            e1 = (s16)p->occCount;
            AmapOccAdd(p, 0x83, -1, d2, 1);
            e2 = (s16)p->occCount;
            AmapOccAdd(p, 0x84, -1, type, 0);
            e3 = (s16)p->occCount;
            AmapOccAdd(p, 0x84, -1, d2, 0);
            cur->areaId = p->areaCount;
            cur->flags = 0;
            cur->ops[k] = 0x101;
            cur->args[k++] = p->occCount;
            if (fp == 9) {
                AmapOccAdd(p, 0x64, -1, idx, 0);
            } else {
                AmapOccAdd(p, 0x68, -1, idx, 0);
            }
            cur->ops[k] = 0x101;
            cur->args[k] = e0;
            pa[2] = 1;
            pb[2] = 20;
            pa[3] = 0x101;
            pb[3] = e1;
            pa[4] = 1;
            pb[4] = 40;
            pa[5] = 0x101;
            pb[5] = e2;
            pa[6] = 1;
            pb[6] = 30;
            pa[7] = 0x101;
            pb[7] = e3;
            pa[8] = 1;
            pb[8] = 180;
            pa[9] = 0x101;
            pb[9] = p->occCount;
            AmapOccAdd(p, 0x6E, -1, p->areaCount, 0);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0xFF, p->areaCount, -1, 0xDB6);
            AmapGimmickBoxGet(0x49, &amapGimmickPos);
            AmapAreaAddBox(p, 6, -1, &amapGimmickPos);
            p->objCount++;
            p->occsCount++;
            break;
        case 25:
            ENT_SET(type);
            idx = p->objCount; p->objCount = idx + 1;
            AmapOccAdd(p, 0x79, -1, w4, 0);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x78, p->areaCount, idx, 0);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x7A, p->areaCount, w4, 0);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x7B, p->areaCount, w4, 0);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x7C, p->areaCount, w4, 0);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x7D, p->areaCount, w4, 0);
            AmapGimmickBoxGet(0xC8, &amapGimmickPos);
            AmapAreaAddBox(p, 4, -1, &amapGimmickPos);
            break;
        case 29:
            ENT_SET(type);
            idx = p->objCount;
            AmapGimmickBoxGet(p->mapType == 4 ? 0x5E8 : 0x6E8, &amapGimmickPos);
            AmapAreaAddBox(p, 0xA, -1, &amapGimmickPos);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 7, p->areaCount, idx, 0);
            AmapAreaAddBox(p, 1, -1, &amapGimmickPos);
            AmapAreaAddBox(p, 0xB, -1, &amapGimmickPos);
            p->objCount++;
            break;
        case 30:
            ENT_SET(type);
            idx = p->objCount;
            AmapGimmickBoxGet(p->mapType == 4 ? 0x5F0 : 0x6F0, &amapGimmickPos);
            AmapAreaAddBox(p, 0xA, -1, &amapGimmickPos);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 7, p->areaCount, idx, 0);
            AmapAreaAddBox(p, 1, -1, &amapGimmickPos);
            AmapAreaAddBox(p, 0xB, -1, &amapGimmickPos);
            p->objCount++;
            break;
        }
    }

    switch (p->mapType) {
    case 1:
    case 2:
        AmapAreaAdd(p, 0, 0, 0, 0, p->startX * 500 + 250, 0, p->startY * 500 - 170, -1);
        AmapAreaAdd(p, 0, 0, 0, p->startY * 500 - 250, p->startX * 500 - 170, 0, p->height * 500, -1);
        AmapAreaAdd(p, 0, p->startX * 500 - 250, 0, p->startY * 500 + 170, p->width * 500, 0, p->height * 500, -1);
        AmapAreaAdd(p, 0, p->startX * 500 + 170, 0, 0, p->width * 500, 0, p->startY * 500 + 250, -1);
        break;
    case 0:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        AmapAreaAdd(p, 0, 0, 0, 0, p->exitX * 500 + 250, 0, p->exitY * 500 - 170, -1);
        AmapAreaAdd(p, 0, 0, 0, p->exitY * 500 - 250, p->exitX * 500 - 170, 0, p->height * 500, -1);
        AmapAreaAdd(p, 0, p->exitX * 500 - 250, 0, p->exitY * 500 + 170, p->width * 500, 0, p->height * 500, -1);
        AmapAreaAdd(p, 0, p->exitX * 500 + 170, 0, 0, p->width * 500, 0, p->exitY * 500 + 250, -1);
        break;
    }

    for (i = 0; i < p->groups[0].gateCount; i++) {
        t4 = p->groups[0].gates[i].area;
        w4 = p->groups[0].gates[i].id;
        for (jj = 0; jj < p->groups[0].gateCount; jj++) {
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x16, t4, p->groups[0].gates[jj].id, 0);
        }
        for (j = 0; j < p->groups[0].switchCount; j++) {
            v = p->groups[0].switches[j].id;
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x11, t4, v, 0);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x13, t4, v, 0);
            ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x17, t4, v, 0);
        }
        for (jj = 0; jj < p->groups[0].gateCount; jj++) {
            if (i != jj) {
                ((void (*)(AmapWork *, s32, s32, s32, s32))AmapOccAdd)(p, 0x18, t4, p->groups[0].gates[jj].area, 0);
            }
        }
        AmapOccAdd(p, 0x19, -1, w4, i);
    }
}
