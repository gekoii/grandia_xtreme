#include "common.h"
#include "protos/main.h"
#include "sys.h"


typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0x94];
} Item;

typedef struct {
    s32 count;
    u8 pad4[0x4C];
    Item *items;
} List;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[0x28];
} Loc;


extern u8 sysPathPartyPac[];

void SysPartyPacLoad(void) {
    Loc loc;
    s32 a;
    s32 b;
    s32 c;
    s32 i;
    s32 n;
    List *l;
    Item *it;

    MemHeapSelect(sysState.dataHeap);
    a = MemAlloc64(FileGetSizeAligned(sysPathPartyPac));
    FileRead(sysPathPartyPac, a);
    c = FileCmpGetSize(a);
    MemHeapSelect(sysState.heap38);
    b = MemAlloc64(c);
    FileDecompress(a, b, 1);
    sysPartyPac.offsets = (s32 *)b;
    for (i = 0; i < 9; i++) {
        sysPartyPac.offsets[i] += b;
        sysPartyPac.packs[i] = FilePacCharRelocate(sysPartyPac.offsets[i]);
    }
    ChrPackBind(&loc, sysPartyPac.packs[6]);
    l = FilePacGetModel(loc.unk4, 0);
    n = l->count;
    it = l->items;
    while (n > 0) {
        n--;
        if (it->unk4 == 0x302) {
            it->unk8 &= 0xFFFFFFF7;
        }
        it++;
    }
    MemHeapReset(sysState.dataHeap);
}
