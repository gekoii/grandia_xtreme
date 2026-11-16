#include "common.h"
#include "protos/field.h"
#include "field/event.h"

void *memset(void *, s32, u32);

void EvtScnrDebugSetCmd(u16 cmd, u8 *p) {
    u32 *a[6];
    s16 v;
    u8 *q;

    a[0] = &evtScnrWork->dbgArgs[0];
    a[1] = &evtScnrWork->dbgArgs[1];
    a[2] = &evtScnrWork->dbgArgs[2];
    a[3] = &evtScnrWork->dbgArgs[3];
    a[4] = &evtScnrWork->dbgArgs[4];
    a[5] = &evtScnrWork->dbgArgs[5];
    memset(a[0], 0, 0x18);
    evtScnrWork->dbgCmd = cmd;
    if (p == NULL) {
        return;
    }
    switch (cmd) {
    case 0:
    case 1:
    case 2:
        break;
    case 3:
        *a[0] = p[0];
        EvtScnrReadU16(p + 1, &v);
        *a[0] = (u16)v;
        break;
    case 6:
        q = EvtScnrReadU16(p, &v);
        *a[0] = (u16)v;
        *a[1] = *q;
        break;
    case 7:
        {
            u8 c = *p;
            q = p + 1;
            *a[0] = c;
            switch (*a[0]) {
            case 3:
                *a[1] = *q++;
                *a[2] = *q++;
                *a[3] = *q++;
                *a[4] = *q++;
                break;
            case 5:
                *a[1] = *q++;
                *a[2] = *q++;
                break;
            case 0:
            case 1:
            case 2:
            case 4:
            case 6:
            case 7:
                break;
            }
        }
        break;
    case 9:
        *a[0] = p[0];
        EvtScnrReadU16(p + 1, &v);
        *a[1] = (u16)v;
        break;
    case 11:
        EvtScnrReadU32(p, a[0]);
        break;
    case 13:
        {
            u8 c = *p;
            q = p + 1;
            *a[0] = c;
            switch (*a[0]) {
            case 0:
                q = EvtScnrReadU16(q, &v);
                *a[1] = (u16)v;
                q = EvtScnrReadU16(q, &v);
                *a[2] = (u16)v;
                q = EvtScnrReadU16(q, &v);
                *a[3] = (u16)v;
                EvtScnrReadU16(q, &v);
                *a[4] = (u16)v;
                break;
            case 2:
                *a[1] = *q;
                break;
            case 3:
                *a[1] = *q;
                break;
            case 1:
            case 4:
            case 5:
            case 6:
                break;
            }
        }
        break;
    case 14:
        {
            *a[0] = *p;
            q = p + 1;
            switch (*a[0]) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                break;
            case 5:
                EvtScnrReadU16(q, &v);
                *a[1] = (u16)v;
                break;
            case 6:
                *a[1] = p[1];
                EvtScnrReadU16(p + 2, &v);
                *a[2] = (u16)v;
                break;
            }
        }
        break;
    case 15:
        {
            u8 c = *p;
            q = p + 1;
            *a[0] = c;
            *a[1] = c >> 5;
            *a[2] = (*a[0] & 0x18) >> 3;
            *a[3] = *a[0] & 7;
            if (*a[0] < 8) {
                EvtScnrReadU16(q, &v);
                *a[3] = (u16)v;
            }
        }
        break;
    case 16:
        *a[0] = p[0];
        q = EvtScnrReadU16(p + 1, &v);
        *a[1] = (u16)v;
        q = EvtScnrReadU16(q, &v);
        *a[2] = (u16)v;
        EvtScnrReadU16(q, &v);
        *a[3] = (u16)v;
        break;
    case 17:
        *a[0] = p[0];
        EvtScnrReadU16(p + 1, &v);
        *a[1] = (u16)v;
        break;
    case 10:
    case 19:
        *a[0] = p[0];
        EvtScnrReadU32(p + 1, a[1]);
        break;
    case 20:
        *a[0] = p[0];
        *a[1] = p[1];
        q = EvtScnrReadU16(p + 2, &v);
        *a[2] = (u16)v;
        *a[3] = *q;
        break;
    case 21:
        *a[0] = p[0];
        *a[1] = p[1];
        *a[2] = p[2];
        *a[3] = p[3];
        break;
    case 22:
        {
            u8 c = *p;
            q = p + 1;
            *a[0] = c;
            switch (*a[0]) {
            case 0:
                EvtScnrReadU16(q, &v);
                *a[1] = (u16)v;
                *a[2] = *(u8 *)a[0];
                break;
            case 1:
                EvtScnrReadU16(q, &v);
                *a[1] = (u16)v;
                break;
            case 16:
                *a[1] = *q;
                q = EvtScnrReadU16(q + 1, &v);
                *a[2] = (u16)v;
                q = EvtScnrReadU16(q, &v);
                *a[3] = (u16)v;
                q = EvtScnrReadU16(q, &v);
                *a[4] = (u16)v;
                *a[5] = *q;
                break;
            case 17:
                *a[1] = *q;
                break;
            case 18:
                *a[1] = *q;
                break;
            case 19:
                *a[1] = *q;
                break;
            case 20:
                *a[1] = *q++;
                *a[2] = *q++;
                EvtScnrReadU16(q, &v);
                *a[3] = (u16)v;
                break;
            case 21:
                *a[1] = *q++;
                *a[2] = *q++;
                q = EvtScnrReadU16(q, &v);
                *a[3] = (u16)v;
                EvtScnrReadU16(q, &v);
                *a[4] = (u16)v;
                break;
            case 32:
            case 33:
                q = EvtScnrReadU16(q, &v);
                *a[1] = (u16)v;
                EvtScnrReadU16(q, &v);
                *a[2] = (u16)v;
                break;
            case 34:
                q = EvtScnrReadU16(q, &v);
                *a[1] = (u16)v;
                *a[2] = q[0];
                *a[3] = q[1];
                break;
            }
        }
        break;
    case 5:
    case 8:
    case 24:
        *a[0] = p[0];
        *a[1] = p[1];
        break;
    case 29:
        {
            *a[0] = *p;
            q = p + 1;
            if (*a[0] != 0xFF) {
                EvtScnrReadU16(q, &v);
                *a[1] = (u16)v;
            }
        }
        break;
    case 4:
    case 12:
    case 18:
    case 23:
    case 28:
        *a[0] = p[0];
        break;
    }
}
