#include "common.h"
#include "protos/main.h"
#include "main/n10.h"

typedef struct {
    s32 id;
    s32 f4;
    s32 count;
    s32 fC;
} Hdr;

typedef struct {
    u8 pad0[6];
    s16 a;
    u8 pad8[6];
    u16 b;
} SubEnt;

extern s32 sndChannelValue[];
extern u8 sndBankHdr[];
extern u8 sndBankHdrIds[];

s32 SndStrmGetReady(s32, s32);

s32 SndPrepare(SndPrepareReq *req) {
    s32 tmp;
    SndIndexEntry *e;
    s8 type;
    SndHandle *obj;
    s32 ch;
    s32 i;
    Hdr *hdr;
    s32 flag;
    s32 *idx;
    SubEnt *sub;

    e = &sndIndexTable[req->index];
    type = e->type & 0x7F;
    switch (type) {
    case 4:
        obj = SndFindHandle(req->handle);
        if (obj == NULL) {
            return -3;
        }
        if (sndStrmSlots[e->bank + 2].busy != 0) {
            return -2;
        }
        if (obj->state > 0) {
            return -2;
        }
        i = SndStrmIndexFind(e->bank, e->u.packed, &tmp);
        if (i == -4) {
            return -1;
        }
        if (e->bank >= 2) {
            return -2;
        }
        sndStrmSlots[e->bank + 2].busy = 1;
        if (SndIopCmdPush(0x208, e->bank, (i & 0xFFFFF) + sndStrmDataLsn, tmp) != 0) {
            obj->state = 1;
            obj->type = type;
            obj->unk7 = 0;
            obj->unk6 = e->unk1;
            obj->chan = e->bank;
            obj->voice = i < 0;
            obj->strmSlot = e->bank + 2;
            obj->id = req->handle;
            obj->fadeSpeed = 0;
            obj->duration = -1;
            obj->targetVol = 10000;
            obj->curVol = 10000;
            break;
        }
        return -11;
    case 9:
        ch = e->bank & 0xF;
        e = sndIndexTable;
        sndBankSlot[ch] = 0;
        while (1) {
            if ((e->type & 0x7F) == 5 && (e->bank & 0xF) == ch) {
                e->type &= 0x7F;
            }
            if (e->type == -1) {
                break;
            }
            e++;
        }
        if (req->handle < 0) {
            SndIopCmdPush(0x20B, 0, ch, 0);
            return 0;
        }
        if (SndStrmGetReady(req->unk8, 0) < req->unkC) {
            SndIopCmdPush(0x20B, 0, ch, 0);
            return -10;
        }
        SndStrmFetchStart(req->unk8, sndBankHdr, 2);
        do {
            for (i = 0xFFF; i >= 0; i--) {
            }
        } while (SndIopIsRecvDone() != 1);
        sndStrmFetchSlot = -16;
        if (SndStrmFetch(req->unk8, sndBankHdr, 1) <= 0) {
            return -10;
        }
        hdr = (Hdr *)((u32)sndBankHdr | 0x20000000);
        flag = 0;
        if (hdr->id != req->index) {
            if (hdr->id >= 0) {
                return -1;
            }
            flag = 1;
        }
        i = 0;
        SndIopCmdPush(0x20A, req->unk8, ch, req->unkC);
        idx = (s32 *)((u32)sndBankHdrIds | 0x20000000);
        sub = (SubEnt *)((u32)(sndBankHdrIds + 0x7F0) | 0x20000000);
        do {
            sndIndexTable[*idx].type |= 0x80;
            tmp = sndIndexTable[*idx].bank & 0xE0;
            if (flag) {
                sndIndexTable[*idx].bank = tmp | ch;
                sndIndexTable[*idx].slot = i;
            }
            tmp = sub->a;
            if (tmp <= 0) {
                tmp = 0;
            } else {
                tmp &= 0xFFFF;
            }
            i++;
            sndIndexTable[*idx].u.packed = tmp | ((sub->b & 0x7FFF) << 16);
            sub++;
            idx++;
        } while (i < hdr->count);
        sndBankSlot[ch] = req->unk8 + 1;
        return ch;
    case 5:
        obj = SndFindHandle(req->handle);
        if (obj == NULL) {
            return -7;
        }
        if (obj->state != 0) {
            return -2;
        }
        if (!(e->type & 0x80)) {
            return -1;
        }
        obj->type = type;
        obj->fadeSpeed = 0;
        obj->targetVol = 10000;
        obj->curVol = 10000;
        obj->index = req->index;
        obj->id = req->handle;
        obj->state = 1;
        break;
    case 11:
        ch = e->bank;
        sndChannelValue[ch] = req->unkC;
        break;
    case 10:
        break;
    default:
        return -1;
    }
    return 0;
}
