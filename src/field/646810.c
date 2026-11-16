#include "common.h"
#include "protos/field.h"
#include "field/n24.h"

void MemSet(void *, s32, s32);

void MiniConcertDataLoad(void) {
    u32 p;
    u32 i;

    MemSet(miniConcertData, 0, 0x94);
    if (MiniConcertGetDataFile() != 0) {
        miniConcertData->hdr = (MiniConcertHdr *)MiniConcertGetDataFile();
        miniConcertData->unk4 = (u8 *)MiniAlignUp((u32)miniConcertData->hdr + 0x18, 0x10);
        miniConcertData->unk8 = (u16 *)MiniAlignUp((u32)miniConcertData->unk4 + miniConcertData->hdr->unk5 * 5, 0x10);
        miniConcertData->unkC = (f32 *)MiniAlignUp((u32)miniConcertData->unk8 + miniConcertData->hdr->unk6 * 2, 0x10);
        miniConcertData->unk10 = (u8 *)MiniAlignUp((u32)miniConcertData->unkC + miniConcertData->hdr->unk7 * 32, 0x10);
        miniConcertData->pos = (MiniConcertPos *)MiniAlignUp((u32)miniConcertData->unk10 + miniConcertData->hdr->unk7, 0x10);
        miniConcertData->patterns = (MiniConcertPattern *)MiniAlignUp((u32)miniConcertData->pos + miniConcertData->hdr->unk8 * 64, 0x10);
        miniConcertData->unk1C = MiniAlignUp((u32)miniConcertData->patterns + 0x30, 0x10);
        miniConcertData->unk20 = (u8 (*)[3])MiniAlignUp(miniConcertData->unk1C + miniConcertData->hdr->unkA * 64, 0x10);
        miniConcertData->unk24 = (u8 (*)[9])MiniAlignUp((u32)miniConcertData->unk20 + 0x1E, 0x10);
        miniConcertData->camPath = (MiniConcertCamPath *)MiniAlignUp((u32)miniConcertData->unk24 + 0x2D, 0x10);
        miniConcertData->unk2C = (u8 *)MiniAlignUp((u32)miniConcertData->camPath + 0xD0, 0x10);
        miniConcertData->unk30 = (void *)MiniAlignUp((u32)miniConcertData->unk2C + miniConcertData->hdr->unkB * 32, 0x10);
        miniConcertData->unk34 = MiniAlignUp((u32)miniConcertData->unk30 + miniConcertData->hdr->unkC * 8, 0x10);
        miniConcertData->unk38 = MiniAlignUp(miniConcertData->unk34 + miniConcertData->hdr->unkD * 32, 0x10);
        miniConcertData->unk3C = MiniAlignUp(miniConcertData->unk38 + miniConcertData->hdr->unkE * 8, 0x10);
        miniConcertData->unk40 = MiniAlignUp(miniConcertData->unk3C + miniConcertData->hdr->unkF * 32, 0x10);
        miniConcertData->rules = (MiniConcertRules *)MiniAlignUp(miniConcertData->unk40 + miniConcertData->hdr->unk10 * 8, 0x10);
        p = (u32)miniConcertData->rules + 0x20;
        for (i = 0; i < 5; i++) {
            miniConcertData->songs[i] = (MiniConcertSong *)MiniAlignUp(p, 0x10);
            miniConcertData->spots[i] = (MiniConcertSpot *)MiniAlignUp((u32)miniConcertData->songs[i] + 0x30, 0x10);
            miniConcertData->vibs[i] = (MiniConcertVib *)MiniAlignUp((u32)miniConcertData->spots[i] + miniConcertData->songs[i]->unk20 * 4, 0x10);
            p = (u32)miniConcertData->vibs[i] + miniConcertData->songs[i]->unk22 * 8;
        }
        miniConcertData->info.unk0 = miniConcertData->hdr->unk0;
        miniConcertData->info.unk4 = miniConcertData->hdr->unk4;
        miniConcertData->info.unk5 = miniConcertData->hdr->unk11;
        miniConcertData->info.unk6 = miniConcertData->hdr->unk12;
        miniConcertData->info.unk7 = miniConcertData->hdr->unk13;
        miniConcertData->info.unk8 = miniConcertData->hdr->unk14;
        miniConcertData->info.unk9 = miniConcertData->hdr->unk15;
        MiniLayoutLoad(&miniConcertData->info, p, miniConcertData->hdr);
    } else {
        miniConcertWork->state = 0;
    }
}
