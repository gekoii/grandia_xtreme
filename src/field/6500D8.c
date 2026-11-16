/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "save.h"
#include "protos/field.h"
#include "field/n25.h"

typedef struct {
    f32 unk0;
    u8 unk4;
    u8 pad5[3];
} Prob6500D8;

void MiniBallCourseSelect(void) {
    s32 n;
    s32 i;
    s32 k;
    MiniBallCourse *e;
    u32 p;
    Prob6500D8 *q;
    f32 r;
    u32 s4;
    u32 s5;
    u32 s6;

    s6 = 0;
    s5 = 0;
    s4 = 0;
    n = sysSaveData.miniBallCourse;
    if (n <= 0) return;
    if (miniBallData->hdr->courseCount < n) return;
    e = &miniBallData->courses[n - 1];
    p = MiniAlignUp((u32)&miniBallData->courses[miniBallData->hdr->courseCount], 0x10);
    miniBallWork->game.course = n;
    miniBallWork->game.cost = e->cost;
    for (i = 0; i < n; i++) {
        q = (Prob6500D8 *)MiniAlignUp(p, 0x10);
        p = MiniAlignUp((u32)q + miniBallData->courses[i].probCount * 8, 0x10);
    }
    r = VuRandSigned(1.0f) + 0.5f;
    for (i = 0; i < e->probCount; i++) {
        if (r < q[i].unk0) break;
    }
    if (i == e->probCount) {
        k = e->defaultSet;
    } else {
        k = q[i].unk4;
    }
    p = miniBallData->sets;
    if (k <= 0 || miniBallData->hdr->setCount < k) {
        k = 1;
    }
    for (i = 0; i < k; i++) {
        s4 = MiniAlignUp(p, 0x10);
        s6 = MiniAlignUp(s4 + 0x18, 0x10);
        s5 = MiniAlignUp(s6 + 0x24, 0x10);
        p = MiniAlignUp(s5 + ((MiniBallSet *)s4)->gustCount * 16, 0x10);
    }
    miniBallData->reelTimes = (u16 *)s6;
    miniBallData->set = (MiniBallSet *)s4;
    miniBallData->gusts = (MiniBallGust *)s5;
    if (((MiniBallSet *)s4)->gustMode == 2) {
        miniBallWork->game.gustLeft = ((MiniBallGust *)s5)->len;
        miniBallWork->game.gustWait = ((MiniBallGust *)s5)->start;
    } else {
        miniBallWork->game.gustWait = 360;
        miniBallWork->game.gustLeft = 0;
    }
    miniBallWork->plays.b[0] = sysSaveData.unk1F41[(n - 1) * 2 + 10];
    miniBallWork->plays.b[1] = sysSaveData.unk1F41[(n - 1) * 2 + 11];
    miniBallWork->plays.n++;
    if (miniBallWork->plays.n >= 10000) {
        miniBallWork->plays.n = 9999;
    }
    sysSaveData.unk1F41[(n - 1) * 2 + 10] = miniBallWork->plays.b[0];
    sysSaveData.unk1F41[(n - 1) * 2 + 11] = miniBallWork->plays.b[1];
}
