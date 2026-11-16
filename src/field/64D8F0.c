/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n25.h"

void MiniPanelsUpdate(MiniPanel *arr, u32 n) {
    u32 i;
    u8 fl;

    for (i = 0; i < n; i++) {
        if (arr[i].active == 0) {
            continue;
        }
        fl = arr[i].flags;
        if (fl == 0) {
            continue;
        }
        if (fl & 1) {
            arr[i].x = arr[i].startX + (arr[i].goalX - arr[i].startX) * ((f32)(arr[i].moveCount + 1) / (f32)arr[i].moveTime);
            arr[i].y = arr[i].startY + (arr[i].goalY - arr[i].startY) * ((f32)(arr[i].moveCount + 1) / (f32)arr[i].moveTime);
        }
        if (fl & 2) {
            arr[i].alpha = ((f32)(arr[i].moveCount + 1) / (f32)arr[i].moveTime) * 128.0f;
        }
        if (fl & 4) {
            arr[i].alpha = 128.0f - ((f32)(arr[i].moveCount + 1) / (f32)arr[i].moveTime) * 128.0f;
        }
        if (++arr[i].moveCount == arr[i].moveTime) {
            if (fl & 4) {
                arr[i].active = 0;
            }
            arr[i].moveCount = 0;
            arr[i].flags = 0;
        }
    }
}
