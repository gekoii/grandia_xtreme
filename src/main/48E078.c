/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 pad[0x10];
    s32 idx;
    s32 val;
} A0;

typedef struct {
    u8 pad[0x1C];
    s32 *arr;
} A1;

void _sceSifCmdSetSreg(A0 *arg0, A1 *arg1) {
    arg1->arr[arg0->idx] = arg0->val;
}
