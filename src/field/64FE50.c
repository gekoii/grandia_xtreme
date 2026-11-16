#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

void MemSet(void *, s32, s32);

void MiniBallDataLoad(void) {
    u32 p;
    u32 q;
    u32 r;
    u32 i;

    MemSet(miniBallData, 0, 0x44);
    if (MiniBallGetDataFile() != NULL) {
        miniBallData->hdr = MiniBallGetDataFile();
        miniBallData->courses = (MiniBallCourse *)MiniAlignUp((u32)miniBallData->hdr + 0x10, 0x10);
        p = MiniAlignUp((u32)miniBallData->courses + miniBallData->hdr->courseCount * 8, 0x10);
        for (i = 0; i < miniBallData->hdr->courseCount; i++) {
            p = MiniAlignUp(MiniAlignUp(p, 0x10) + miniBallData->courses[i].probCount * 8, 0x10);
        }
        miniBallData->sets = MiniAlignUp(p, 0x10);
        for (i = 0; i < miniBallData->hdr->setCount; i++) {
            q = MiniAlignUp(p, 0x10);
            p = MiniAlignUp(MiniAlignUp(MiniAlignUp(q + 0x18, 0x10) + 0x24, 0x10) + ((MiniBallSet *)q)->gustCount * 16, 0x10);
        }
        miniBallData->walls = (MiniBallWall *)MiniAlignUp(p, 0x10);
        miniBallData->panels = (MiniPanelSrc *)MiniAlignUp((u32)miniBallData->walls + miniBallData->hdr->wallCount * 8, 0x10);
        miniBallData->cells = (MiniCell *)MiniAlignUp((u32)miniBallData->panels + miniBallData->hdr->panelCount * 32, 0x10);
        r = MiniAlignUp((u32)miniBallData->cells + miniBallData->hdr->cellCount * 8, 0x10);
        miniBallData->params = (MiniBallParams *)r;
        miniBallData->layoutHdr.msgSize = miniBallData->hdr->msgSize;
        miniBallData->layoutHdr.groupCount = miniBallData->hdr->groupCount;
        miniBallData->layoutHdr.rectCount = miniBallData->hdr->rectCount;
        miniBallData->layoutHdr.idxCount = miniBallData->hdr->idxCount;
        miniBallData->layoutHdr.listCount = miniBallData->hdr->listCount;
        miniBallData->layoutHdr.imageCount = miniBallData->hdr->imageCount;
        miniBallData->layoutHdr.texCount = miniBallData->hdr->texCount;
        MiniLayoutLoad(&miniBallData->layoutHdr, r + 0x58, miniBallData->hdr);
    } else {
        miniBallWork->state = 0;
    }
}
