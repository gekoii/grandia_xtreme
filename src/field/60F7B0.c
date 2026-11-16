#include "common.h"
#include "protos/field.h"

typedef struct {
    u8 b[6];
} Col6;


extern u32 amapItemLogPos;
extern char amapItemRollLogFmt[];
extern u16 amapItemRollLogId[];
extern Col6 amapItemRollLog[];

void AmapDbgItemLogDraw(s32 arg0, s16 mode) {
    u32 i;

    switch (mode) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        for (i = 0; i < amapItemLogPos; i++) {
            DbgPrintf(3, i + 4, amapItemRollLogFmt, amapItemRollLog[i].b[0], amapItemRollLog[i].b[1], amapItemRollLog[i].b[2],
                          amapItemRollLog[i].b[3], amapItemRollLog[i].b[4], amapItemRollLog[i].b[5], amapItemRollLogId[i]);
        }
        break;
    }
}
