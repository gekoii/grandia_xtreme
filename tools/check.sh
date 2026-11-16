#!/bin/sh
# Compare each rebuilt binary with the original.
cd "$(dirname "$0")/.."
status=0
check() { # built original
    if cmp -s "$1" "$2"; then
        echo "OK       $1"
    else
        echo "MISMATCH $1 (vs $2)"
        cmp "$1" "$2" | head -1
        status=1
    fi
}
check build/SLUS_204.17.rom rom/SLUS_204.17.rom
check build/field.bin iso/FIELD.BIN
check build/battle.bin iso/BATTLE.BIN
check build/viewer.bin iso/VIEWER.BIN
# Every C file must be in its binary's segment list; otherwise the binary is
# built from assembly and the comparison above proves nothing about the C.
for pair in SLUS_204.17:main field:field battle:battle viewer:viewer; do
    cfg=${pair%%:*}; dir=${pair#*:}
    files=$(ls src/$dir/*.c | grep -vc '/bss_')
    segs=$(sed -n '/BEGIN text/,/END text/p' config/$cfg.yaml | grep -c ', c, ')
    if [ "$files" != "$segs" ]; then
        echo "MISMATCH config/$cfg.yaml lists $segs C files, src/$dir has $files (run tools/update_segments.py $dir)"
        status=1
    fi
    # The data-only C files (plan step 6b) are .data/.rodata subsegments
    # (tools/update_segments.py)
    for f in $(ls src/$dir/data/*.c 2>/dev/null); do
        stem=$(basename "$f" .c)
        if ! grep -Eq ", \.(ro)?data, $dir/data/$stem\]" config/$cfg.yaml; then
            echo "MISMATCH $f is not a .data/.rodata subsegment in config/$cfg.yaml (run tools/update_segments.py $dir)"
            status=1
        fi
    done
    # The .bss files (tools/gen_bss.py) are .bss subsegments, listed by hand
    for f in $(ls src/$dir/bss_*.c 2>/dev/null); do
        stem=$(basename "$f" .c)
        if ! grep -Eq "(, |type: )\.bss, (vram: 0x[0-9A-Fa-f]+, name: )?$dir/$stem([] ]|\$)" config/$cfg.yaml; then
            echo "MISMATCH $f is not a .bss subsegment in config/$cfg.yaml"
            status=1
        fi
    done
done
exit $status
