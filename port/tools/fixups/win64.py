"""The game's C on x86-64 (port/docs/win64.md).

On x86-64 every pointer field of a game struct is a 32-bit EE address
(port/tools/ee32.py rewrites the uses). What it can't rewrite is settled
here, written so that i686 compiles the same code: GRX_EE_PTR(T) is a T *
there and GRX_EE_TO(T, x) a cast to the type x already has
(port/include/grx_ee64.h).

- Locals and parameters that walk or point into an array of pointers in a
  game struct (T **p = &s->arr[i], a T ** parameter given s->arr) become
  GRX_EE_PTR(T) *: pointers to the 32-bit entries, gone through with
  GRX_EE_TO.
- Structs with the hardware's layout and the heap's block header got
  GRX_EE_PTR fields before the rewrite existed (libdma's local copies,
  MemBlock); the rewrite leaves fields that aren't pointers alone. A
  comparison of such a field with (void *)-1 is made between 32-bit
  values: the same on i686.
"""

# T **pp = &chrPortraitSlots.slots[i]: ChrPortraitSet 40EB28,
# ChrPortraitSetByIndex 40EC10, ChrPortraitSetByCode 40ECC0
_PORTRAIT_PP = [
    (r"ChrPortrait \*\*pp;", "GRX_EE_PTR(ChrPortrait) *pp;"),
    # every *pp but the declaration's
    (r"(?<![\w*])\*pp\b(?!;)", "GRX_EE_TO(ChrPortrait, *pp)"),
]

# libdma's sceDmaSend 46E3C0 and sceDmaSendN 46E498: their local view of
# sceDmaChan
_DMA_CHAN = [
    (r"    void \*madr;\n", "    GRX_EE_PTR(void) madr;\n"),
    (r"    void \*tadr;\n", "    GRX_EE_PTR(void) tadr;\n"),
]

HEADER_FIXUPS = {
    # The heap's block header: MemAlloc also writes it by offsets from the
    # user pointer (self at -0x14), so it keeps the PS2's 0x20 bytes
    "include/main/n06.h": [
        (r"/\* 0x00 \*/ struct MemBlock \*next;", "/* 0x00 */ GRX_EE_PTR(struct MemBlock) next;"),
        (r"/\* 0x0C \*/ struct MemBlock \*self;", "/* 0x0C */ GRX_EE_PTR(struct MemBlock) self;"),
    ],
    # EffListAnyVisible 5E0290 takes one of EffMapWork's emitter pools
    "include/protos/field.h": [
        (r"s32 EffListAnyVisible\(struct PtclEmitter \*\*, s32\);",
         "s32 EffListAnyVisible(GRX_EE_PTR(struct PtclEmitter) *, s32);"),
    ],
}

FIXUPS = {
    # MemGetFreeMax 4404E0 and MemFree 440668: through a block's next
    "src/main/4404E0.c": [
        (r"head->next->size", "GRX_EE_TO(MemBlock, head->next)->size"),
        (r"head->next->next", "GRX_EE_TO(MemBlock, head->next)->next"),
    ],
    "src/main/440668.c": [
        # Its view of the 0x14 bytes before an allocation (MemBlock.self
        # and the guard): 0x14 bytes on x86-64 too
        (r"typedef struct \{ MemBlock \*bp;", "typedef struct { GRX_EE_PTR(MemBlock) bp;"),
        (r"bp->size \+= p->next->size;", "bp->size += GRX_EE_TO(MemBlock, p->next)->size;"),
        (r"bp->next = p->next->next;", "bp->next = GRX_EE_TO(MemBlock, p->next)->next;"),
    ],
    "src/main/40EB28.c": _PORTRAIT_PP,
    "src/main/40EC10.c": _PORTRAIT_PP,
    "src/main/40ECC0.c": _PORTRAIT_PP,
    # BtlChrMotionGetEvent: walks the unit's evtArgs
    "src/battle/6AB148.c": [
        (r"s16 \*\*b = arg0->evtArgs;", "GRX_EE_PTR(s16) *b = arg0->evtArgs;"),
        (r"\*arg2 = \*b;", "*arg2 = GRX_EE_TO(s16, *b);"),
    ],
    # MgcModelRegister: stores into mgcWork.models through q
    "src/battle/6B19E0.c": [
        (r"s32 \*\*q = \(s32 \*\*\)m->models;", "GRX_EE_PTR(s32) *q = (GRX_EE_PTR(s32) *)m->models;"),
    ],
    # EffListAnyVisible: through one of EffMapWork's emitter pools
    "src/field/5E0290.c": [
        (r"s32 EffListAnyVisible\(PtclEmitter \*\*arg0, s32 arg1\)",
         "s32 EffListAnyVisible(GRX_EE_PTR(PtclEmitter) *arg0, s32 arg1)"),
        (r"arg0\[i\] != NULL && arg0\[i\]->unk12B9", "arg0[i] != 0 && GRX_EE_TO(PtclEmitter, arg0[i])->unk12B9"),
    ],
    # ChrTenantSetup 5E1AA8: fills the object's tracks and areas through va
    # and qq (stores only)
    "src/field/5E1AA8.c": [
        (r"struct ChrTenantTrack \*\(\*va\)\[7\];", "GRX_EE_PTR(struct ChrTenantTrack) (*va)[7];"),
        (r"struct ChrTenantArea \*\*qq;", "GRX_EE_PTR(struct ChrTenantArea) *qq;"),
    ],
    # MiniConcertCarrosGet 646200 fills miniConcertWork->carros (stores
    # only); MiniConcertStart 6466A0 declares it too
    "src/field/646200.c": [
        (r"void MiniConcertCarrosGet\(ChrObj \*out\[5\]\[5\]\)", "void MiniConcertCarrosGet(GRX_EE_PTR(ChrObj) out[5][5])"),
    ],
    "src/field/6466A0.c": [
        (r"void MiniConcertCarrosGet\(ChrObj \*out\[5\]\[5\]\);", "void MiniConcertCarrosGet(GRX_EE_PTR(ChrObj) out[5][5]);"),
    ],
    # ChrModelCreate 4381D8: stores the part array as a 32-bit value
    "src/main/4381D8.c": [
        (r"\*\(void \*\*\)&obj->parts = chrModelCreatePartPtr;",
         "*(GRX_EE_PTR(void) *)&obj->parts = chrModelCreatePartPtr;"),
    ],
    # 478DA0: the current decoder's sys, read through a volatile pointer
    "src/main/478DA0.c": [
        (r"sub = \*\(SceMpegSys \*volatile \*\)&_sceMpegCurMp->sys;",
         "sub = GRX_EE_TO(SceMpegSys, *(GRX_EE_PTR(SceMpegSys) volatile *)&_sceMpegCurMp->sys);"),
    ],
    # SndDriverInit: its arguments are SndBgmChannel's 32-bit drvIndex and
    # drvInitData (the caller passes &sndBgmChannels.ent[0].sub)
    "src/main/460B60.c": [
        (r"\(SndIndexEntry \*\)arg0\[0\]", "(SndIndexEntry *)GRX_EE_TO(char, ((GRX_EE_PTR(char) *)arg0)[0])"),
        (r"SndIniReadLine\(line, arg0\[1\]\)", "SndIniReadLine(line, GRX_EE_TO(char, ((GRX_EE_PTR(char) *)arg0)[1]))"),
    ],
    # MapMiniTenantIcons 601138: the first field of the enemy's sense work
    # (32 bits now) read as an Anim *
    "src/field/601138.c": [
        (r"\(\*\(Anim \*\*\)e->sense\)", "GRX_EE_TO(Anim, *(GRX_EE_PTR(Anim) *)e->sense)"),
    ],
    # DbgFldScenarioListPage: evtScnrWork->index (a 32-bit field) against -1
    "src/field/5D73E8.c": [
        (r"if \(p == \(u16 \*\)-1\)", "if ((u32)(__UINTPTR_TYPE__)p == 0xFFFFFFFFu)"),
    ],
    "src/main/46E3C0.c": _DMA_CHAN + [
        (r"if \(d->tadr != \(void \*\)-1\)", "if ((u32)d->tadr != (u32)-1)"),
    ],
    "src/main/46E498.c": _DMA_CHAN + [
        (r"if \(d->madr != \(void \*\)-1\)", "if ((u32)d->madr != (u32)-1)"),
    ],
}
