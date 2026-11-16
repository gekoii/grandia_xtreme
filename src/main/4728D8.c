/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 (*func)(void *, s32 *, s32);
    s32 arg;
} Entry;

typedef struct {
    u8 pad[0xC];
    Entry e[1];
} Table;

typedef struct {
    u8 pad[0x40];
    Table *table;
} Obj;

s32 _sceMpegDoCallback(Obj *arg0, s32 *arg1) {
    s32 ret = 0;
    Table *t;
    if (arg0 != NULL) {
        t = arg0->table;
        if (t != NULL) {
            if (t->e[*arg1].func != NULL) {
                ret = t->e[*arg1].func(arg0, arg1, t->e[*arg1].arg);
            }
        }
    }
    return ret;
}
