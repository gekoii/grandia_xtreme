/* Matches newlib __svfscanf (vfscanf.c).
 * Third-party library code: it follows the upstream source and falls under
 * that library's license; see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 *_p;
    s32 _r;
} FILE;

typedef u32 size_t;
typedef u64 u_long;
typedef u8 u_char;

extern const char ctypeTable[];
extern const s16 basefix[];
extern size_t fread(void *, size_t, size_t, FILE *);
extern f32 func_00481AE0(double) __attribute__((const));

#define isspace(c) ((ctypeTable)[(unsigned) (c)] & 8)
#define isupper(c) ((ctypeTable)[(unsigned) (c)] & 1)
#define va_arg(ap, T) ((ap) += 8, *(T *) ((ap) - 8))

#define BUF 350
#define EOF (-1)

#define LONG 0x01
#define LONGDBL 0x02
#define SHORT 0x04
#define SUPPRESS 0x08
#define POINTER 0x10
#define NOSKIP 0x20
#define SIGNOK 0x40
#define NDIGITS 0x80
#define DPTOK 0x100
#define EXPOK 0x200
#define PFXOK 0x100
#define NZDIGITS 0x200

#define CT_CHAR 0
#define CT_CCL 1
#define CT_STRING 2
#define CT_INT 3
#define CT_FLOAT 4

#define BufferEmpty (fp->_r <= 0 && __srefill(fp))

s32 __svfscanf(FILE *fp, const char *fmt0, char *ap) {
    char *fmt = (char *) fmt0;
    s32 c;
    size_t width;
    char *p;
    s32 n;
    s32 flags;
    char *p0;
    s32 nassigned;
    s32 nread;
    s32 base = 0;
    s32 nbytes = 1;
    s32 wc;
    char *lptr;

    u_long (*ccfn)() = 0;
    char ccltab[256];
    char buf[BUF];

    s16 *sp;
    s32 *ip;
    f32 *flp;
    double *dp;
    s64 *lp;

    nassigned = 0;
    nread = 0;
    for (;;) {
        wc = *fmt;
        fmt += nbytes;
        if (wc == 0)
            return nassigned;
        if (nbytes == 1 && isspace(wc)) {
            for (;;) {
                if (BufferEmpty)
                    return nassigned;
                if (!isspace(*fp->_p))
                    break;
                nread++, fp->_r--, fp->_p++;
            }
            continue;
        }
        if (wc != '%')
            goto literal;
        width = 0;
        flags = 0;

    again:
        c = *fmt++;

        switch (c) {
        case '%':
        literal:
            lptr = fmt - nbytes;
            for (n = 0; n < nbytes; ++n) {
                if (BufferEmpty)
                    goto input_failure;
                if (*fp->_p != *lptr)
                    goto match_failure;
                fp->_r--, fp->_p++;
                nread++;
                ++lptr;
            }
            continue;

        case '*':
            flags |= SUPPRESS;
            goto again;
        case 'l':
            flags |= LONG;
            goto again;
        case 'L':
            flags |= LONGDBL;
            goto again;
        case 'h':
            flags |= SHORT;
            goto again;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            width = width * 10 + c - '0';
            goto again;

        case 'D':
            flags |= LONG;
        case 'd':
            c = CT_INT;
            ccfn = (u_long (*)()) strtol;
            base = 10;
            break;

        case 'i':
            c = CT_INT;
            ccfn = (u_long (*)()) strtol;
            base = 0;
            break;

        case 'O':
            flags |= LONG;
        case 'o':
            c = CT_INT;
            ccfn = strtoul;
            base = 8;
            break;

        case 'u':
            c = CT_INT;
            ccfn = strtoul;
            base = 10;
            break;

        case 'X':
        case 'x':
            flags |= PFXOK;
            c = CT_INT;
            ccfn = strtoul;
            base = 16;
            break;

        case 'E':
        case 'G':
        case 'e':
        case 'f':
        case 'g':
            c = CT_FLOAT;
            break;

        case 's':
            c = CT_STRING;
            break;

        case '[':
            fmt = __sccl(ccltab, fmt);
            flags |= NOSKIP;
            c = CT_CCL;
            break;

        case 'c':
            flags |= NOSKIP;
            c = CT_CHAR;
            break;

        case 'p':
            flags |= POINTER | PFXOK;
            c = CT_INT;
            ccfn = strtoul;
            base = 16;
            break;

        case 'n':
            if (flags & SUPPRESS)
                continue;
            if (flags & SHORT) {
                sp = va_arg(ap, s16 *);
                *sp = nread;
            } else if (flags & LONG) {
                lp = va_arg(ap, s64 *);
                *lp = nread;
            } else {
                ip = va_arg(ap, s32 *);
                *ip = nread;
            }
            continue;

        case '\0':
            return EOF;

        default:
            if (isupper(c))
                flags |= LONG;
            c = CT_INT;
            ccfn = (u_long (*)()) strtol;
            base = 10;
            break;
        }

        if (BufferEmpty)
            goto input_failure;

        if ((flags & NOSKIP) == 0) {
            while (isspace(*fp->_p)) {
                nread++;
                if (--fp->_r > 0)
                    fp->_p++;
                else if (__srefill(fp))
                    goto input_failure;
            }
        }

        switch (c) {
        case CT_CHAR:
            if (width == 0)
                width = 1;
            if (flags & SUPPRESS) {
                size_t sum = 0;

                for (;;) {
                    if ((n = fp->_r) < width) {
                        sum += n;
                        width -= n;
                        fp->_p += n;
                        if (__srefill(fp)) {
                            if (sum == 0)
                                goto input_failure;
                            break;
                        }
                    } else {
                        sum += width;
                        fp->_r -= width;
                        fp->_p += width;
                        break;
                    }
                }
                nread += sum;
            } else {
                size_t r = fread((void *) va_arg(ap, char *), 1, width, fp);
                if (r == 0)
                    goto input_failure;
                nread += r;
                nassigned++;
            }
            break;

        case CT_CCL:
            if (width == 0)
                width = ~0;
            if (flags & SUPPRESS) {
                n = 0;
                while (ccltab[*fp->_p]) {
                    n++, fp->_r--, fp->_p++;
                    if (--width == 0)
                        break;
                    if (BufferEmpty) {
                        if (n == 0)
                            goto input_failure;
                        break;
                    }
                }
                if (n == 0)
                    goto match_failure;
            } else {
                p0 = p = va_arg(ap, char *);
                while (ccltab[*fp->_p]) {
                    fp->_r--;
                    *p++ = *fp->_p++;
                    if (--width == 0)
                        break;
                    if (BufferEmpty) {
                        if (p == p0)
                            goto input_failure;
                        break;
                    }
                }
                n = p - p0;
                if (n == 0)
                    goto match_failure;
                *p = 0;
                nassigned++;
            }
            nread += n;
            break;

        case CT_STRING:
            if (width == 0)
                width = ~0;
            if (flags & SUPPRESS) {
                n = 0;
                while (!isspace(*fp->_p)) {
                    n++, fp->_r--, fp->_p++;
                    if (--width == 0)
                        break;
                    if (BufferEmpty)
                        break;
                }
                nread += n;
            } else {
                p0 = p = va_arg(ap, char *);
                while (!isspace(*fp->_p)) {
                    fp->_r--;
                    *p++ = *fp->_p++;
                    if (--width == 0)
                        break;
                    if (BufferEmpty)
                        break;
                }
                *p = 0;
                nread += p - p0;
                nassigned++;
            }
            continue;

        case CT_INT:
            if (--width > sizeof(buf) - 2)
                width = sizeof(buf) - 2;
            width++;
            flags |= SIGNOK | NDIGITS | NZDIGITS;
            for (p = buf; width; width--) {
                c = *fp->_p;
                switch (c) {
                case '0':
                    if (base == 0) {
                        base = 8;
                        flags |= PFXOK;
                    }
                    if (flags & NZDIGITS)
                        flags &= ~(SIGNOK | NZDIGITS | NDIGITS);
                    else
                        flags &= ~(SIGNOK | PFXOK | NDIGITS);
                    goto ok;

                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                    base = basefix[base];
                    flags &= ~(SIGNOK | PFXOK | NDIGITS);
                    goto ok;

                case '8':
                case '9':
                    base = basefix[base];
                    if (base <= 8)
                        break;
                    flags &= ~(SIGNOK | PFXOK | NDIGITS);
                    goto ok;

                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                    if (base <= 10)
                        break;
                    flags &= ~(SIGNOK | PFXOK | NDIGITS);
                    goto ok;

                case '+':
                case '-':
                    if (flags & SIGNOK) {
                        flags &= ~SIGNOK;
                        goto ok;
                    }
                    break;

                case 'x':
                case 'X':
                    if (flags & PFXOK && p == buf + 1) {
                        base = 16;
                        flags &= ~PFXOK;
                        goto ok;
                    }
                    break;
                }
                break;
            ok:
                *p++ = c;
                if (--fp->_r > 0)
                    fp->_p++;
                else if (__srefill(fp))
                    break;
            }
            if (flags & NDIGITS) {
                if (p > buf)
                    (void) ungetc(*--p, fp);
                goto match_failure;
            }
            c = p[-1];
            if (c == 'x' || c == 'X') {
                --p;
                ungetc(c, fp);
            }
            if ((flags & SUPPRESS) == 0) {
                u_long res;

                *p = 0;
                res = (*ccfn)(buf, (char **) NULL, base);
                if (flags & POINTER)
                    *(va_arg(ap, void **)) = (void *) (u32) res;
                else if (flags & SHORT) {
                    sp = va_arg(ap, s16 *);
                    *sp = res;
                } else if (flags & LONG) {
                    lp = va_arg(ap, s64 *);
                    *lp = res;
                } else {
                    ip = va_arg(ap, s32 *);
                    *ip = res;
                }
                nassigned++;
            }
            nread += p - buf;
            break;

        case CT_FLOAT:
            if (--width > sizeof(buf) - 2)
                width = sizeof(buf) - 2;
            width++;
            flags |= SIGNOK | NDIGITS | DPTOK | EXPOK;
            for (p = buf; width; width--) {
                c = *fp->_p;
                switch (c) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    flags &= ~(SIGNOK | NDIGITS);
                    goto fok;

                case '+':
                case '-':
                    if (flags & SIGNOK) {
                        flags &= ~SIGNOK;
                        goto fok;
                    }
                    break;
                case '.':
                    if (flags & DPTOK) {
                        flags &= ~(SIGNOK | DPTOK);
                        goto fok;
                    }
                    break;
                case 'e':
                case 'E':
                    if ((flags & (NDIGITS | EXPOK)) == EXPOK) {
                        flags = (flags & ~(EXPOK | DPTOK)) | SIGNOK | NDIGITS;
                        goto fok;
                    }
                    break;
                }
                break;
            fok:
                *p++ = c;
                if (--fp->_r > 0)
                    fp->_p++;
                else if (__srefill(fp))
                    break;
            }
            if (flags & NDIGITS) {
                if (flags & EXPOK) {
                    while (p > buf)
                        ungetc(*--p, fp);
                    goto match_failure;
                }
                c = *--p;
                if (c != 'e' && c != 'E') {
                    (void) ungetc(c, fp);
                    c = *--p;
                }
                (void) ungetc(c, fp);
            }
            if ((flags & SUPPRESS) == 0) {
                double res;

                *p = 0;
                res = atof(buf);
                if (flags & LONG) {
                    dp = va_arg(ap, double *);
                    *dp = res;
                } else if (flags & LONGDBL) {
                    dp = va_arg(ap, double *);
                    *dp = res;
                } else {
                    flp = va_arg(ap, f32 *);
                    *flp = func_00481AE0(res);
                }
                nassigned++;
            }
            nread += p - buf;
            break;
        }
    }
input_failure:
    return nassigned ? nassigned : -1;
match_failure:
    return nassigned;
}
