"""Compares two directories of per-vsync pictures (vsync_NNNNNN.ppm, from
port/tests/gs_replay.c -f and port/tests/gs_gl_replay.c -f): how much each
frame differs, and the worst frames side by side as PNG (reference, test,
and the difference amplified) to look at.

  python port/tests/gs_compare.py REF_DIR TEST_DIR [-o OUT_DIR] [-w N]

Per frame: the mean absolute difference over the colour channels, and the
share of pixels whose largest channel difference is over 8, 32 and 96. A
test picture at another size (an internal scale) is box-filtered down to
the reference's size first. Needs numpy and Pillow. The pictures are game
data: keep them and the output under build/."""
import argparse
import glob
import os
import sys

import numpy as np
from PIL import Image


def read_ppm(path):
    with open(path, "rb") as f:
        data = f.read()
    # P6\n<w> <h>\n255\n
    parts = data.split(b"\n", 3)
    w, h = map(int, parts[1].split())
    return np.frombuffer(parts[3], dtype=np.uint8, count=w * h * 3).reshape(h, w, 3)


def fit(img, shape):
    h, w = shape[:2]
    if img.shape[:2] == (h, w):
        return img
    kh, kw = img.shape[0] // h, img.shape[1] // w
    if kh * h == img.shape[0] and kw * w == img.shape[1]:
        return img.reshape(h, kh, w, kw, 3).mean(axis=(1, 3)).round().astype(np.uint8)
    return np.asarray(Image.fromarray(img).resize((w, h), Image.BOX))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("ref")
    ap.add_argument("test")
    ap.add_argument("-o", dest="out", help="write the worst frames side by side here")
    ap.add_argument("-w", dest="worst", type=int, default=6, help="how many of the worst frames (default 6)")
    ap.add_argument("-q", dest="quiet", action="store_true", help="the summary only")
    args = ap.parse_args()

    rows = []
    for rp in sorted(glob.glob(os.path.join(args.ref, "vsync_*.ppm"))):
        name = os.path.basename(rp)
        tp = os.path.join(args.test, name)
        if not os.path.exists(tp):
            print(f"{name}: missing from {args.test}")
            continue
        r = read_ppm(rp)
        t = fit(read_ppm(tp), r.shape)
        d = np.abs(r.astype(np.int16) - t.astype(np.int16))
        dm = d.max(axis=2)
        rows.append((name, float(d.mean()), float((dm > 8).mean()) * 100, float((dm > 32).mean()) * 100,
                     float((dm > 96).mean()) * 100, int(dm.max())))
    if not rows:
        sys.exit("no frames to compare")
    if not args.quiet:
        print(f"{'frame':22} {'mean':>6} {'>8 %':>7} {'>32 %':>7} {'>96 %':>7} {'max':>4}")
        for n, m, a, b, c, x in rows:
            print(f"{n:22} {m:6.2f} {a:7.2f} {b:7.2f} {c:7.2f} {x:4d}")
    ms = np.array([r[1] for r in rows])
    a8 = np.array([r[2] for r in rows])
    a32 = np.array([r[3] for r in rows])
    print(f"{len(rows)} frames: mean abs diff {ms.mean():.3f} (worst {ms.max():.3f}); pixels off by >8: "
          f"{a8.mean():.2f}% (worst {a8.max():.2f}%), by >32: {a32.mean():.2f}% (worst {a32.max():.2f}%); "
          f"identical frames {sum(1 for r in rows if r[5] == 0)}")
    if args.out:
        os.makedirs(args.out, exist_ok=True)
        worst = sorted(rows, key=lambda r: -r[3] - r[1])[: args.worst]
        for n, m, a, b, c, x in worst:
            r = read_ppm(os.path.join(args.ref, n))
            tfull = read_ppm(os.path.join(args.test, n))
            t = fit(tfull, r.shape)
            d = np.abs(r.astype(np.int16) - t.astype(np.int16)).max(axis=2)
            heat = np.clip(d * 4, 0, 255).astype(np.uint8)
            heat = np.stack([heat, heat // 2, heat // 4], axis=2)
            img = np.concatenate([r, t, heat], axis=1)
            out = os.path.join(args.out, n.replace(".ppm", ".png"))
            Image.fromarray(img).save(out)
            print(f"wrote {out} (reference | test | difference x4)")


if __name__ == "__main__":
    main()
