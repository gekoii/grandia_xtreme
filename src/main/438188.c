#include "common.h"
#include "main/n05.h"

#define ALIGN64(x) (((x) + 0x3F) & 0xFFFFFFC0)

/* One parameter: PtclGenCalcSize's cast call passes three more, which only steer its register choice. */
s32 ChrModelCalcInstanceSize(ChrModelData *arg0) {
    s32 a = arg0->partCount;
    s32 b = arg0->gaObjCount;
    s32 size = (a << 6) + 0xD0;
    size += ALIGN64(a * 0xA0);
    size += ALIGN64(a << 7);
    size += ALIGN64(b << 7);
    return size;
}
