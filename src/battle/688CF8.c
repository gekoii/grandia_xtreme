#include "common.h"

s32 BtlResultExpStep(s32 n) {
    n = n / 100;
    if (n >= 10) {
        n = n / 10 * 10;
        n++;
    } else if (n <= 0) {
        n = 1;
    }
    return n;
}
