#include "common.h"
#include "pad.h"
#include "sys.h"




f32 DbgEditValue(s32 mode, s32 base, s32 max, f32 val) {
    u16 btn;
    Pad *pad = &padPort1;
    s32 n;

    if (pad->held & 0x20) {
        if (pad->repeat & 0x8000) {
            sysState.dbgDigit++;
        } else if (pad->repeat & 0x2000) {
            sysState.dbgDigit--;
        }
        if (pad->repeat & 0x1000) {
            val = 1.0f;
        } else if (pad->repeat & 0x4000) {
            val = -1.0f;
        } else {
            return 0.0f;
        }
        n = sysState.dbgDigit;
        if (n <= 0) {
            sysState.dbgDigit = 1;
            n = 1;
        }
        if (max < n + base) {
            sysState.dbgDigit = max - base;
            n = max - base;
        }
        base += n - 1;
        switch (mode) {
        case 0:
            for (; base > 0; base--) {
                val *= 10.0f;
            }
            break;
        case 1:
            for (; base > 0; base--) {
                val *= 16.0f;
            }
            break;
        case 2:
            if (base < 0) {
                base = -base;
                do {
                    val *= 0.1f;
                    base--;
                } while (base != 0);
            } else {
                for (; base > 0; base--) {
                    val *= 10.0f;
                }
            }
            break;
        case 3:
            val = 1.0f;
            for (; base > 0; base--) {
                val *= 2.0f;
            }
            break;
        }
        return val;
    }
    btn = pad->repeat;
    if (btn & 0xA000) {
        if (mode == 3) {
            return 1.0f;
        }
        if (btn & 0x8000) {
            return -val;
        }
        return val;
    }
    return 0.0f;
}
