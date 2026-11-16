#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "gph.h"
#include "party.h"

/*
 * SysDataInit stores about 30 table pointers from SYS/GRXSYS.PAC into
 * globals. gcse hoists each global's %hi and numbers the new pseudos in
 * hash-bucket order, and that hash comes from the symbol's name
 * (docs/naming.md, "Hash order"), so this function's register allocation
 * depended on the original, unknown names. The globals are written here
 * under their address-derived names, whose hashes give the original order;
 * everywhere else they have their real names:
 *   D_004A33C8 = sysPath6500Se
 *   D_004A9430 = ptyEggFusionTable
 *   D_004A9438 = sysDropItemTable
 *   D_004A9478 = sysGbxPac
 *   D_004A947C = ptySkillBookTypeTable
 *   D_004A33D8 = sysPathFaceDat
 *   D_004A9440 = ptyItemTable
 *   D_004A9444 = ptyPartyData
 *   D_004A944C = ptyGrowthTable
 *   D_004A33A0 = sysPathVmsInit
 *   D_004A9480 = ptySkillTable
 *   D_004F0D00 = sysState
 *   D_004A9488 = ptyComboTable
 *   D_004A9418 = sysGrxTable7C
 *   D_004A941C = ptyManaEggTable
 *   D_004A9450 = ptyCharDefTable
 *   D_004A3378 = sysPathGrxsysPac
 *   D_004A945C = sysGrxsysPac
 *   D_004A33B8 = sysPathSysSe
 *   D_004A9494 = ptyMoveTable
 *   D_004EF198 = ptyCharStatsBuf
 *   D_004A9420 = sysGrxTableA0
 *   D_004A9424 = sysDropRankTable
 *   D_004A9428 = ptyCharData
 *   D_004ED5C0 = sysTexUploadPacket
 *   D_004A942C = ptyItemSortKeys
 *   D_004A9460 = sysCombineTable
 *   D_004A3388 = sysPathSndIdx
 */
typedef struct {
    u8 pad0[8];
    u8 *x8;
    u8 *xC;
} SubC8;
typedef struct {
    s32 x0;
    s32 x4;
    s32 x8;
    s32 xC;
    s32 x10;
    s32 x14;
    s32 x18;
    s32 x1C;
    s32 x20;
    s32 x24;
    s32 x28;
    s32 x2C;
    s32 x30;
    s32 x34;
    s32 x38;
    s32 x3C;
    s32 x40;
    s32 x44;
    s32 x48;
} SubC4;
typedef struct {
    s32 x0;
    s32 x4;
    s32 x8;
    s32 xC;
    s32 x10;
    s32 x14;
    s32 x18;
    s32 x1C;
    s32 x20;
    s32 x24;
    s32 x28;
    s32 x2C;
    s32 x30;
    s32 x34;
    s32 x38;
    s32 x3C;
    s32 x40;
    s32 x44;
    s32 x48;
    s32 x4C;
    s32 x50;
    s32 x54;
    s32 x58;
    s32 x5C;
    s32 x60;
    s32 x64;
    s32 x68;
    s32 x6C;
    s32 x70;
    s32 x74;
    s32 x78;
    s32 x7C;
    s32 x80;
    s32 x84;
    s32 x88;
    s32 x8C;
    s32 x90;
    s32 x94;
    s32 x98;
    s32 x9C;
    s32 xA0;
    s32 xA4;
    s32 xA8;
    s32 xAC;
    s32 xB0;
    s32 xB4;
    s32 xB8;
    s32 xBC;
    s32 xC0;
    SubC4 *xC4;
    SubC8 *xC8;
    s32 xCC;
    s32 xD0;
    s32 xD4;
    s32 xD8;
    s32 xDC;
    s32 xE0;
    s32 xE4;
    s32 xE8;
    s32 xEC;
    s32 xF0;
    s32 xF4;
    s32 xF8;
    s32 xFC;
    s32 x100;
    s32 x104;
    s32 x108;
    s32 x10C;
    s32 x110;
    s32 x114;
    s32 x118;
    s32 x11C;
    s32 x120;
    s32 x124;
    s32 x128;
    s32 x12C;
    s32 x130;
    s32 x134;
    s32 x138;
    s32 x13C;
    s32 x140;
    s32 x144;
    s32 x148;
    s32 x14C;
    s32 x150;
    s32 x154;
    s32 x158;
    s32 x15C;
} Hdr;

extern Hdr *D_004A945C;
extern SysState D_004F0D00;
extern s32 vuMtxStackBuf;
extern s32 D_004A9480;
extern s32 D_004A9488;
extern u8 gsMainPacketBuf[];
extern u8 D_004ED5C0[];
extern char sysPathGbxPac[];
extern char D_004A3378[];
extern char D_004A3388[];
extern char D_004A33A0[];
extern char D_004A33B8[];
extern char D_004A33C8[];
extern char D_004A33D8[];
extern s32 *D_004A9478;
extern s32 D_004A947C;
extern s32 D_004A9430;
extern s32 D_004A9418;
extern s32 D_004A9460;
extern s32 D_004A9440;
extern s32 D_004A941C;
extern s32 D_004A9494;
extern s32 D_004A942C;
extern s32 D_004A944C;
extern s32 D_004A9450;
extern s32 D_004A9424;
extern s32 D_004A9438;
extern s32 D_004A9420;
extern u8 *D_004A9428;
extern u8 *D_004A9444;
extern u8 D_004EF198[];

void VuMtxStackInit(s32, s32);
void MemSet(s32, s32, s32);
void SysTableRelocate(s32);

void SysDataInit(void) {
    u32 n;
    s32 buf;
    s32 hdr;
    u32 i;
    s32 a;
    s32 b;
    s32 pa;
    s32 pb;
    s32 k;
    s32 tmp;
    SubC4 *q;
    TxtFontImage *o;

    D_004A945C = NULL;
    MemHeapSelect(D_004F0D00.heap30);
    D_004F0D00.screenCaptureAddr = MemAlloc128(0x120000);
    VuMtxStackSet(&vuMtxStackBuf);
    VuMtxStackInit(0x40, MemAlloc32(0x1000));
    VuMtxPoolInit(0x1000, MemAlloc32(0x40000));
    gphTexTableBuf = (GphTexTable *)MemAlloc32(GphTexTableSize(0x100));
    GphTexTableInit(0x100, (s32)gphTexTableBuf);
    GphTexTableSelect(gphTexTableBuf);
    ptyBattleParams = (PtyEnemyParams *)MemAlloc128(0x2A0);
    ptyPartyUnits = (PtyUnit *)MemAlloc128(0x3180);
    ptyCharCalcUnits = (PtyUnit *)MemAlloc128(0x18C0);
    MemSet((s32)ptyBattleParams, 0, 0x2A0);
    MemSet((s32)ptyPartyUnits, 0, 0x3180);
    MemSet((s32)ptyCharCalcUnits, 0, 0x18C0);
    n = MemAlloc32(0x180000);
    GsPacketBufInit(gsMainPacketBuf, 0x180000, n);
    GphShatterInit();
    SysDemoReset();
    MapSetFileReset();
    logf(0.1f);
    n = FileGetSizeAligned(sysPathGbxPac);
    if (n != 0) {
        MemHeapSelect(D_004F0D00.heap30);
        buf = MemAlloc32(n);
        FileRead(sysPathGbxPac, buf);
        D_004A9478 = (s32 *)buf;
        n = *(u32 *)buf >> 2;
        for (i = 0; i < n; i++) {
            { s32 *p = &D_004A9478[i]; *p += buf; }
        }
    }
    n = FileGetSizeAligned(D_004A3378);
    if (n != 0) {
        MemHeapSelect(D_004F0D00.dataHeap);
        buf = MemAlloc32(n);
        FileRead(D_004A3378, buf);
        MemHeapSelect(D_004F0D00.heap30);
        FileCmpGetSize(buf);
        hdr = MemAlloc32(FileCmpGetSize(buf));
        FileDecompress(buf, hdr, 1);
        D_004A945C = (Hdr *)hdr;
        MemHeapSelect(D_004F0D00.dataHeap);
        MemFree(buf);
        MemHeapSelect(D_004F0D00.heap30);
        tmp = D_004A945C->xB0 + (s32)D_004A945C;
        D_004A945C->xB0 = tmp;
        D_004A945C->x4 = D_004A945C->x4 + (s32)D_004A945C;
        D_004A945C->x0 = D_004A945C->x0 + (s32)D_004A945C;
        D_004A945C->x8 = D_004A945C->x8 + (s32)D_004A945C;
        D_004A945C->xC = D_004A945C->xC + (s32)D_004A945C;
        D_004A945C->x10 = D_004A945C->x10 + (s32)D_004A945C;
        D_004A945C->x14 = D_004A945C->x14 + (s32)D_004A945C;
        D_004A945C->x18 = D_004A945C->x18 + (s32)D_004A945C;
        D_004A945C->x1C = D_004A945C->x1C + (s32)D_004A945C;
        D_004A945C->x20 = D_004A945C->x20 + (s32)D_004A945C;
        D_004A945C->x24 = D_004A945C->x24 + (s32)D_004A945C;
        D_004A945C->x28 = D_004A945C->x28 + (s32)D_004A945C;
        D_004A945C->x2C = D_004A945C->x2C + (s32)D_004A945C;
        D_004A945C->x38 = D_004A945C->x38 + (s32)D_004A945C;
        D_004A945C->x40 = D_004A945C->x40 + (s32)D_004A945C;
        D_004A945C->x50 = D_004A945C->x50 + (s32)D_004A945C;
        D_004A945C->x54 = D_004A945C->x54 + (s32)D_004A945C;
        D_004A945C->xC8 = (SubC8 *)((u8 *)D_004A945C + (s32)D_004A945C->xC8);
        D_004A945C->x58 = D_004A945C->x58 + (s32)D_004A945C;
        D_004A945C->x4C = D_004A945C->x4C + (s32)D_004A945C;
        D_004A945C->x5C = D_004A945C->x5C + (s32)D_004A945C;
        D_004A945C->x60 = D_004A945C->x60 + (s32)D_004A945C;
        D_004A945C->x64 = D_004A945C->x64 + (s32)D_004A945C;
        D_004A945C->x68 = D_004A945C->x68 + (s32)D_004A945C;
        D_004A945C->xBC = D_004A945C->xBC + (s32)D_004A945C;
        D_004A945C->xB4 = D_004A945C->xB4 + (s32)D_004A945C;
        D_004A945C->xC0 = D_004A945C->xC0 + (s32)D_004A945C;
        D_004A945C->xB8 = (s32)D_004A945C + D_004A945C->xB8;
        D_004A945C->xC8->xC = (u8 *)D_004A945C->xC8 + (s32)D_004A945C->xC8->xC;
        D_004A945C->xC8->x8 = (u8 *)D_004A945C->xC8 + (s32)D_004A945C->xC8->x8;
        D_004A945C->xCC = D_004A945C->xCC + (s32)D_004A945C;
        D_004A945C->xD0 = D_004A945C->xD0 + (s32)D_004A945C;
        D_004A945C->xD4 = D_004A945C->xD4 + (s32)D_004A945C;
        D_004A945C->xD8 = D_004A945C->xD8 + (s32)D_004A945C;
        D_004A945C->xDC = D_004A945C->xDC + (s32)D_004A945C;
        D_004A945C->xE0 = D_004A945C->xE0 + (s32)D_004A945C;
        D_004A945C->xE4 = D_004A945C->xE4 + (s32)D_004A945C;
        D_004A945C->xE8 = D_004A945C->xE8 + (s32)D_004A945C;
        D_004A945C->xEC = D_004A945C->xEC + (s32)D_004A945C;
        D_004A945C->xF0 = D_004A945C->xF0 + (s32)D_004A945C;
        D_004A945C->xF4 = D_004A945C->xF4 + (s32)D_004A945C;
        D_004A945C->xF8 = D_004A945C->xF8 + (s32)D_004A945C;
        D_004A945C->xFC = D_004A945C->xFC + (s32)D_004A945C;
        D_004A945C->x100 = D_004A945C->x100 + (s32)D_004A945C;
        D_004A945C->x104 = D_004A945C->x104 + (s32)D_004A945C;
        D_004A945C->x108 = D_004A945C->x108 + (s32)D_004A945C;
        D_004A945C->x10C = D_004A945C->x10C + (s32)D_004A945C;
        D_004A945C->x110 = D_004A945C->x110 + (s32)D_004A945C;
        D_004A945C->x114 = D_004A945C->x114 + (s32)D_004A945C;
        D_004A945C->x118 = D_004A945C->x118 + (s32)D_004A945C;
        D_004A945C->x11C = D_004A945C->x11C + (s32)D_004A945C;
        D_004A945C->x120 = D_004A945C->x120 + (s32)D_004A945C;
        D_004A945C->x124 = D_004A945C->x124 + (s32)D_004A945C;
        D_004A945C->x128 = D_004A945C->x128 + (s32)D_004A945C;
        D_004A945C->x12C = D_004A945C->x12C + (s32)D_004A945C;
        D_004A945C->x130 = D_004A945C->x130 + (s32)D_004A945C;
        D_004A945C->x134 = D_004A945C->x134 + (s32)D_004A945C;
        D_004A945C->x138 = D_004A945C->x138 + (s32)D_004A945C;
        D_004A945C->x13C = D_004A945C->x13C + (s32)D_004A945C;
        D_004A945C->x140 = D_004A945C->x140 + (s32)D_004A945C;
        D_004A945C->x144 = D_004A945C->x144 + (s32)D_004A945C;
        D_004A945C->x148 = D_004A945C->x148 + (s32)D_004A945C;
        D_004A945C->x14C = D_004A945C->x14C + (s32)D_004A945C;
        D_004A945C->x44 = D_004A945C->x44 + (s32)D_004A945C;
        D_004A945C->xAC = D_004A945C->xAC + (s32)D_004A945C;
        D_004A945C->x30 = D_004A945C->x30 + (s32)D_004A945C;
        D_004A945C->x34 = D_004A945C->x34 + (s32)D_004A945C;
        D_004A945C->x48 = D_004A945C->x48 + (s32)D_004A945C;
        D_004A945C->x150 = D_004A945C->x150 + (s32)D_004A945C;
        D_004A945C->x154 = D_004A945C->x154 + (s32)D_004A945C;
        D_004A945C->x158 = D_004A945C->x158 + (s32)D_004A945C;
        D_004A945C->x15C = D_004A945C->x15C + (s32)D_004A945C;
        FilePacModelRelocate(tmp);
        FilePacModelRelocate(D_004A945C->xE8);
        FilePacModelRelocate(D_004A945C->x148);
        FileChunkRelocate(D_004A945C->xEC);
        FileChunkRelocate(D_004A945C->xF0);
        FileChunkRelocate(D_004A945C->xF4);
        FileChunkRelocate(D_004A945C->x138);
        FileChunkRelocate(D_004A945C->x13C);
        D_004A945C->xEC += 0x10;
        D_004A945C->xF0 += 0x10;
        D_004A945C->xF4 += 0x10;
        D_004A945C->x138 += 0x10;
        D_004A945C->x13C += 0x10;
        GphTexDataRelocate(D_004A945C->x0);
        GphTexDataRelocate(D_004A945C->x8);
        GphTexDataRelocate(D_004A945C->xC);
        GphTexDataRelocate(D_004A945C->x10);
        GphTexDataRelocate(D_004A945C->x14);
        GphTexDataRelocate(D_004A945C->x18);
        GphTexDataRelocate(D_004A945C->x1C);
        GphTexDataRelocate(D_004A945C->x20);
        GphTexDataRelocate(D_004A945C->x24);
        GphTexDataRelocate(D_004A945C->x28);
        GphTexDataRelocate(D_004A945C->x2C);
        GphTexDataRelocate(D_004A945C->x38);
        GphTexDataRelocate(D_004A945C->x40);
        GphTexDataRelocate(D_004A945C->x54);
        GphTexDataRelocate(D_004A945C->x58);
        GphTexDataRelocate(D_004A945C->x4C);
        GphTexDataRelocate(D_004A945C->x5C);
        GphTexDataRelocate(D_004A945C->x50);
        GphTexDataRelocate(D_004A945C->x60);
        GphTexDataRelocate(D_004A945C->x64);
        GphTexDataRelocate(D_004A945C->x68);
        GphTexDataRelocate(D_004A945C->xBC);
        GphTexDataRelocate(D_004A945C->xCC);
        GphTexDataRelocate(D_004A945C->xD0);
        GphTexDataRelocate(D_004A945C->xD4);
        GphTexDataRelocate(D_004A945C->xD8);
        GphTexDataRelocate(D_004A945C->xDC);
        GphTexDataRelocate(D_004A945C->xE0);
        GphTexDataRelocate(D_004A945C->xE4);
        GphTexDataRelocate(D_004A945C->xF8);
        GphTexDataRelocate(D_004A945C->xFC);
        GphTexDataRelocate(D_004A945C->x100);
        GphTexDataRelocate(D_004A945C->x11C);
        GphTexDataRelocate(D_004A945C->x120);
        GphTexDataRelocate(D_004A945C->x124);
        GphTexDataRelocate(D_004A945C->x128);
        GphTexDataRelocate(D_004A945C->x12C);
        GphTexDataRelocate(D_004A945C->x130);
        GphTexDataRelocate(D_004A945C->x140);
        GphTexDataRelocate(D_004A945C->x44);
        GphTexDataRelocate(D_004A945C->x30);
        GphTexDataRelocate(D_004A945C->x48);
        GphTexDataRelocate(D_004A945C->x150);
        GphTexDataRelocate(D_004A945C->x158);
        GphTexDataRelocate(D_004A945C->x15C);
        GphAnimSetRelocate(D_004A945C->x104);
        GphAnimSetRelocate(D_004A945C->x108);
        GphAnimSetRelocate(D_004A945C->x10C);
        GphAnimSetRelocate(D_004A945C->x110);
        GphAnimSetRelocate(D_004A945C->x114);
        GphAnimSetRelocate(D_004A945C->x118);
        GphAnimSetRelocate(D_004A945C->x34);
        q = (SubC4 *)D_004A945C->x104;
        GphTexDataRelocate(q->xC);
        q = (SubC4 *)D_004A945C->x108;
        GphTexDataRelocate(q->xC);
        q = (SubC4 *)D_004A945C->x10C;
        GphTexDataRelocate(q->xC);
        q = (SubC4 *)D_004A945C->x110;
        GphTexDataRelocate(q->xC);
        q = (SubC4 *)D_004A945C->x118;
        GphTexDataRelocate(q->xC);
        q = (SubC4 *)D_004A945C->x114;
        GphTexDataRelocate(q->xC);
        q = (SubC4 *)D_004A945C->x34;
        GphTexDataRelocate(q->xC);
        D_004A947C = D_004A945C->x6C + (s32)D_004A945C;
        D_004A9430 = D_004A945C->x70 + (s32)D_004A945C;
        D_004A9488 = D_004A945C->x74 + (s32)D_004A945C;
        D_004A9460 = D_004A945C->x78 + (s32)D_004A945C;
        D_004A9418 = D_004A945C->x7C + (s32)D_004A945C;
        D_004A941C = D_004A945C->x80 + (s32)D_004A945C;
        D_004A9440 = D_004A945C->x84 + (s32)D_004A945C;
        D_004A942C = D_004A945C->x88 + (s32)D_004A945C;
        D_004A9494 = D_004A945C->x8C + (s32)D_004A945C;
        D_004A9450 = D_004A945C->x90 + (s32)D_004A945C;
        D_004A944C = D_004A945C->x94 + (s32)D_004A945C;
        D_004A9438 = D_004A945C->x98 + (s32)D_004A945C;
        D_004A9424 = D_004A945C->x9C + (s32)D_004A945C;
        D_004A9420 = D_004A945C->xA0 + (s32)D_004A945C;
        D_004A9480 = D_004A945C->xA4 + (s32)D_004A945C;
        ptyMoveLevelThresholds = (PtyMoveLevelThreshold *)(D_004A945C->xA8 + (s32)D_004A945C);
        PtySysTablesRelocate();
        D_004A9428 = D_004EF198;
        D_004A9444 = D_004A9428 - 0x19EC;
        GphShatterLoad(D_004A945C->x4);
        GsTexLoadPacket(D_004ED5C0, D_004A945C->x8, 0xD3000, 0xD2000, 0, 0, 0, 0, 0, 0, 0, 0);
        DbgTextInit(D_004A945C->xBC);
        ChrPortraitTableInit(D_004A945C->xC0);
        D_004A945C->xC4 = (SubC4 *)((s32)D_004A945C + (s32)D_004A945C->xC4);
        D_004A945C->xC4->x0 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x0;
        D_004A945C->xC4->x4 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x4;
        D_004A945C->xC4->x8 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x8;
        D_004A945C->xC4->xC = (s32)D_004A945C->xC4 + D_004A945C->xC4->xC;
        D_004A945C->xC4->x10 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x10;
        D_004A945C->xC4->x14 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x14;
        D_004A945C->xC4->x18 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x18;
        D_004A945C->xC4->x1C = (s32)D_004A945C->xC4 + D_004A945C->xC4->x1C;
        D_004A945C->xC4->x20 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x20;
        D_004A945C->xC4->x24 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x24;
        D_004A945C->xC4->x28 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x28;
        D_004A945C->xC4->x2C = (s32)D_004A945C->xC4 + D_004A945C->xC4->x2C;
        D_004A945C->xC4->x30 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x30;
        D_004A945C->xC4->x34 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x34;
        D_004A945C->xC4->x38 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x38;
        D_004A945C->xC4->x3C = (s32)D_004A945C->xC4 + D_004A945C->xC4->x3C;
        D_004A945C->xC4->x40 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x40;
        D_004A945C->xC4->x44 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x44;
        D_004A945C->xC4->x48 = (s32)D_004A945C->xC4 + D_004A945C->xC4->x48;
        SysTableRelocate(D_004A945C->xC4->x0);
        SysTableRelocate(D_004A945C->xC4->x4);
        SysTableRelocate(D_004A945C->xC4->x8);
        SysTableRelocate(D_004A945C->xC4->xC);
        SysTableRelocate(D_004A945C->xC4->x10);
        SysTableRelocate(D_004A945C->xC4->x14);
        SysTableRelocate(D_004A945C->xC4->x18);
        SysTableRelocate(D_004A945C->xC4->x1C);
        SysTableRelocate(D_004A945C->xC4->x20);
        SysTableRelocate(D_004A945C->xC4->x24);
        SysTableRelocate(D_004A945C->xC4->x28);
        SysTableRelocate(D_004A945C->xC4->x2C);
        SysTableRelocate(D_004A945C->xC4->x30);
        SysTableRelocate(D_004A945C->xC4->x34);
        SysTableRelocate(D_004A945C->xC4->x38);
        SysTableRelocate(D_004A945C->xC4->x3C);
        SysTableRelocate(D_004A945C->xC4->x40);
        SysTableRelocate(D_004A945C->xC4->x44);
        SysTableRelocate(D_004A945C->xC4->x48);
    }
    a = FileGetSizeAligned(D_004A3388);
    b = FileGetSizeAligned(D_004A33A0);
    if (a != 0 && b != 0) {
        pa = MemAlloc32(a);
        pb = MemAlloc32(b);
        FileRead(D_004A3388, pa);
        FileRead(D_004A33A0, pb);
        SndInit(pa, pb, b);
        SysWaitFrame(0);
        SndSeBankLoad(D_004A33B8, 0, 6000, 0);
        SndSeBankLoad(D_004A33C8, 0, 6500, 0);
    }
    ChrPortraitAllocSlots();
    ChrPortraitLoad(D_004A33D8);
    MemHeapSelect(D_004F0D00.heap30);
    for (k = 0; k < 3; k++) {
        o = &D_004F0D00.fontImages[k];
        buf = MemAlloc64(TxtFontImageCalcSize(o, 0x10, 0x10, 0x200, 0x100));
        TxtFontImageSetup(o, 0x10, 0x10, 0x200, 0x100, D_004A945C->x0, 0, buf);
        TxtFontImageClear(o);
        o->glyphMetricsSrc = (u8 *)D_004A945C->xAC;
    }
}
