#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

void MiniTableRelocate(void *);

void MiniLayoutLoad(MiniLayoutHdr *h, void *p, void *data) {
    u32 i;
    GphAnimSet *o;

    miniLayout->hdr = h;
    miniLayout->groups = MiniAlignUp(p, 0x10);
    miniLayout->msgs = MiniAlignUp(miniLayout->groups + h->groupCount, 0x80);
    MiniTableRelocate(miniLayout->msgs);
    miniLayout->rects = MiniAlignUp(miniLayout->msgs + h->msgSize, 0x10);
    miniLayout->listIdx = MiniAlignUp(miniLayout->rects + h->rectCount, 0x10);
    miniLayout->lists = MiniAlignUp(miniLayout->listIdx + h->idxCount, 0x10);
    for (i = 0; i < h->listCount; i++) {
        if (miniLayout->lists[i].rect == -1) {
            miniLayout->lists[i].rect = 0;
        } else {
            miniLayout->lists[i].rect = (s32)&miniLayout->rects[miniLayout->lists[i].rect];
        }
    }
    miniLayout->images = MiniAlignUp(miniLayout->lists + h->listCount, 0x10);
    miniLayout->imageData = MiniAlignUp(miniLayout->images + h->imageCount, 0x80);
    for (i = 0; i < h->imageCount; i++) {
        o = (GphAnimSet *)(miniLayout->images[i] += (s32)miniLayout->imageData);
        if (i < h->texCount) {
            GphTexDataRelocate(o);
        } else {
            GphAnimSetRelocate(o);
            GphTexDataRelocate(o->tex);
        }
    }
    GphTexRegisterAt((void *)miniLayout->images[0], 0xBE);
}
