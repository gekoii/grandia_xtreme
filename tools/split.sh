#!/bin/sh
# Regenerate the main-ELF rom image and run splat on every config.
set -e
cd "$(dirname "$0")/.."
mkdir -p rom
# asm/ is generated: clear it so files for functions now in C don't linger
rm -rf asm
python3 tools/elf2rom.py iso/SLUS_204.17 rom/SLUS_204.17.rom
for cfg in SLUS_204.17 field battle viewer; do
    .venv/bin/python -m splat split "config/$cfg.yaml"
done
