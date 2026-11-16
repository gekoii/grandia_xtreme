/* Third-party library code (newlib): see README.md, "Third-party library code". */
/* main executable: newlib's malloc state (mallocr.c): the bin array av_
 * (__malloc_av_) and the tunables and statistics after it. Data-only C
 * (plan step 6b, docs/shifting.md); the users are _malloc_r, _free_r,
 * _realloc_r, malloc_extend_top and malloc_trim (488418.c and its
 * neighbours). The 16 bytes after __malloc_current_mallinfo are Sony's
 * libkernel version marker: they are split off into 4A0BF8.c.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x004A07A0-0x004A0BF8 */

/* struct malloc_chunk (0x10), INTERNAL_SIZE_T = 32 bits */
struct malloc_chunk {
    unsigned int prev_size;
    unsigned int size;
    struct malloc_chunk *fd;
    struct malloc_chunk *bk;
};
typedef struct malloc_chunk *mbinptr;

struct mallinfo {
    int arena;    /* total space allocated from system (sbrked_mem) */
    int ordblks;  /* number of non-inuse chunks */
    int smblks;   /* unused -- always zero */
    int hblks;    /* number of mmapped regions */
    int hblkhd;   /* total space in mmapped regions */
    int usmblks;  /* unused -- always zero */
    int fsmblks;  /* unused -- always zero */
    int uordblks; /* total allocated space */
    int fordblks; /* total non-inuse space */
    int keepcost; /* top-most, releasable (via malloc_trim) space */
};

#define SIZE_SZ 4
#define NAV 128

/* Each bin is a fake chunk whose fd/bk are av_[2i+2], av_[2i+3]: bin_at(i)
 * = &av_[2i], and an empty bin points at itself. */
#define bin_at(i) ((mbinptr)((char *)&__malloc_av_[2 * (i) + 2] - 2 * SIZE_SZ))
#define IAV(i) bin_at(i), bin_at(i)

/* av_: top = av_[2] (bin_at(0)'s fd), last_remainder = bin_at(1).
 * fields: D_004A07A8 .. D_004A0B98, every 8 bytes = bin_at(1) .. bin_at(127)
 * (&av_[2] .. &av_[254]) */
mbinptr __malloc_av_[NAV * 2 + 2] = {
    0, 0,
    IAV(0), IAV(1), IAV(2), IAV(3), IAV(4), IAV(5), IAV(6), IAV(7),
    IAV(8), IAV(9), IAV(10), IAV(11), IAV(12), IAV(13), IAV(14), IAV(15),
    IAV(16), IAV(17), IAV(18), IAV(19), IAV(20), IAV(21), IAV(22), IAV(23),
    IAV(24), IAV(25), IAV(26), IAV(27), IAV(28), IAV(29), IAV(30), IAV(31),
    IAV(32), IAV(33), IAV(34), IAV(35), IAV(36), IAV(37), IAV(38), IAV(39),
    IAV(40), IAV(41), IAV(42), IAV(43), IAV(44), IAV(45), IAV(46), IAV(47),
    IAV(48), IAV(49), IAV(50), IAV(51), IAV(52), IAV(53), IAV(54), IAV(55),
    IAV(56), IAV(57), IAV(58), IAV(59), IAV(60), IAV(61), IAV(62), IAV(63),
    IAV(64), IAV(65), IAV(66), IAV(67), IAV(68), IAV(69), IAV(70), IAV(71),
    IAV(72), IAV(73), IAV(74), IAV(75), IAV(76), IAV(77), IAV(78), IAV(79),
    IAV(80), IAV(81), IAV(82), IAV(83), IAV(84), IAV(85), IAV(86), IAV(87),
    IAV(88), IAV(89), IAV(90), IAV(91), IAV(92), IAV(93), IAV(94), IAV(95),
    IAV(96), IAV(97), IAV(98), IAV(99), IAV(100), IAV(101), IAV(102), IAV(103),
    IAV(104), IAV(105), IAV(106), IAV(107), IAV(108), IAV(109), IAV(110), IAV(111),
    IAV(112), IAV(113), IAV(114), IAV(115), IAV(116), IAV(117), IAV(118), IAV(119),
    IAV(120), IAV(121), IAV(122), IAV(123), IAV(124), IAV(125), IAV(126), IAV(127),
};

/* unsigned long is 64-bit on the EE */
unsigned long __malloc_trim_threshold = 0x20000; /* DEFAULT_TRIM_THRESHOLD */
unsigned long __malloc_top_pad = 0;              /* DEFAULT_TOP_PAD */
char *__malloc_sbrk_base = (char *)-1;
unsigned long __malloc_max_sbrked_mem = 0;
unsigned long __malloc_max_total_mem = 0;
struct mallinfo __malloc_current_mallinfo = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
