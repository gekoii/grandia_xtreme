/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "main/n04.h"

void GphDrawListAdd(GphPart *o, GphGaObject *s, s32 m)
{
    GphDrawList *h;
    GphDrawItem *e;
    u32 idx;
    u8 a;
    u64 b;
    FVECTOR v;
    f32 q;
    u16 *l;
    u32 fl;
    u64 k, k2;

    idx = gphDrawListCur->count;
    if (idx < gphDrawListCur->capacity) {
        e = &gphDrawListCur->items[idx];
        b = s->unk3;
        a = o->prio;
        e->texId = b;
        e->gaObj = s;
        e->unk1 = a;
        e->mtx = m;
        *(s16 *)&e->layer = s->clip; /* one sh: layer = clip, unk3 = 0 */
        __asm__ volatile(
            "lqc2 $vf1, 0(%1)\n\t"
            "lqc2 $vf4, 0x0(%2)\n\t"
            "lqc2 $vf5, 0x10(%2)\n\t"
            "lqc2 $vf6, 0x20(%2)\n\t"
            "lqc2 $vf7, 0x30(%2)\n\t"
            "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
            "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
            "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
            "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
            "sqc2 $vf1, 0(%0)"
            : : "r"(v), "r"(s->center), "r"(m) : "memory");
        q = v[2];
        fl = ((o->gsFlags >> 11) & 3) != 0;
        if (q <= 0.0f) {
            q = 65535.0f;
        } else {
            q = 1.0f / q * 65535.0f;
        }
        h = gphDrawListCur;
        k = ((u64)a << 60) | (u32)e;
        k2 = (b << 48) | ((u64)fl << 59);
        k |= k2;
        k |= (u64)(s32)q << 32;
        {
            s32 off = idx * sizeof(u64);
            *(u64 *)((u8 *)h->keys + off) = k;
        }
        h->count = idx + 1;
        if (o->attr & 0x80) {
            l = h->envMapList;
            {
                s32 off = *l * sizeof(u16);
                *(u16 *)((u8 *)l + off + 2) = idx; /* l[1 + *l] */
            }
            (*l)++;
        }
    }
}
