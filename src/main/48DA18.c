/* as: ee-gcc2.9-991111-01 */
/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

extern void (*_scePrintfPutchar)(s32);

#define VA_ARG(ap, T) (*(T *)((ap += 8) - 8))

void _sceVprintf(char *fmt, char *ap) {
    char buf[32];
    char *s;
    char *t;
    char *pad;
    s32 size;
    s32 w;
    s32 c2;
    u64 v;
    s64 sv;
    f32 f;

    while (*fmt != 0) {
        pad = NULL;
        size = 0;
        if (*fmt == '%') {
        again:
            fmt++;
            switch (*fmt) {
                case '0':
                    w = fmt[1] - '0';
                    c2 = fmt[2];
                    if ((u8)w < 10) {
                        if (c2 >= '0' && c2 <= '9') {
                            fmt += 2;
                            w = w * 10 + (c2 - '0');
                            if (w > 31) {
                                w = 31;
                            }
                        } else {
                            fmt++;
                        }
                        pad = &buf[31 - w];
                        while (w > 0) {
                            buf[31 - w] = '0';
                            w--;
                        }
                    }
                    goto again;
                case 'l':
                    size = 'l';
                    goto again;
                case 'h':
                    size = 'h';
                    goto again;
                case 'o':
                    if (size == 'l') {
                        v = VA_ARG(ap, u64);
                    } else if (size == 'h') {
                        v = VA_ARG(ap, u16);
                    } else {
                        v = VA_ARG(ap, u32);
                    }
                    s = &buf[31];
                    *s = 0;
                    if (v == 0) {
                        *--s = '0';
                    } else {
                        while (v != 0) {
                            *--s = (v & 7) + '0';
                            v >>= 3;
                        }
                    }
                    if (pad != NULL && pad < s) {
                        s = pad;
                    }
                    while (*s != 0) {
                        _scePrintfPutchar(*s++);
                    }
                    break;
                case 'x':
                    if (size == 'l') {
                        v = VA_ARG(ap, u64);
                    } else if (size == 'h') {
                        v = VA_ARG(ap, u16);
                    } else {
                        v = VA_ARG(ap, u32);
                    }
                    s = &buf[31];
                    *s = 0;
                    if (v == 0) {
                        *--s = '0';
                    } else {
                        while (v != 0) {
                            u64 d = v & 0xF;
                            if (d < 10) {
                                *--s = d + '0';
                            } else {
                                *--s = d + 'a' - 10;
                            }
                            v >>= 4;
                        }
                    }
                    if (pad != NULL && pad < s) {
                        s = pad;
                    }
                    while (*s != 0) {
                        _scePrintfPutchar(*s++);
                    }
                    break;
                case 'd':
                    if (size == 'l') {
                        sv = VA_ARG(ap, s64);
                    } else if (size == 'h') {
                        sv = VA_ARG(ap, s16);
                    } else {
                        sv = VA_ARG(ap, s32);
                    }
                    s = &buf[31];
                    *s = 0;
                    if (sv == 0) {
                        *--s = '0';
                    } else {
                        if (sv < 0) {
                            sv = -sv;
                            _scePrintfPutchar('-');
                        }
                        while (sv != 0) {
                            *--s = sv % 10 + '0';
                            sv /= 10;
                        }
                    }
                    if (pad != NULL && pad < s) {
                        s = pad;
                    }
                    while (*s != 0) {
                        _scePrintfPutchar(*s++);
                    }
                    break;
                case 'u':
                    if (size == 'l') {
                        v = VA_ARG(ap, u64);
                    } else if (size == 'h') {
                        v = VA_ARG(ap, u16);
                    } else {
                        v = VA_ARG(ap, u32);
                    }
                    s = &buf[31];
                    *s = 0;
                    if (v == 0) {
                        *--s = '0';
                    } else {
                        while (v != 0) {
                            *--s = v % 10 + '0';
                            v /= 10;
                        }
                    }
                    if (pad != NULL && pad < s) {
                        s = pad;
                    }
                    while (*s != 0) {
                        _scePrintfPutchar(*s++);
                    }
                    break;
                case 'e':
                case 'f':
                    f = VA_ARG(ap, f32);
                    if (f == 0.0f) {
                        _scePrintfPutchar('0');
                    } else {
                        _scePrintfDouble(f);
                    }
                    break;
                case 's':
                    t = VA_ARG(ap, char *);
                    if (*t == 0) {
                        _scePrintfPutchar('(');
                        _scePrintfPutchar('n');
                        _scePrintfPutchar('u');
                        _scePrintfPutchar('l');
                        _scePrintfPutchar('l');
                        _scePrintfPutchar(')');
                    } else {
                        u8 *us = (u8 *)t;
                        do {
                            _scePrintfPutchar((char)*us);
                            us++;
                        } while (*us != 0);
                    }
                    break;
                case 'c':
                    sv = VA_ARG(ap, char);
                    _scePrintfPutchar(sv);
                    break;

                default:
                    break;
            }
        } else {
            _scePrintfPutchar(*fmt);
        }
        fmt++;
    }
}
