#!/bin/sh
# Download the candidate EE GCC builds from decomp.me's compiler archive.
set -e
cd "$(dirname "$0")/compilers" 2>/dev/null || { mkdir -p "$(dirname "$0")/compilers"; cd "$(dirname "$0")/compilers"; }
BASE=https://github.com/decompme/compilers/releases/download/compilers
for c in ee-gcc2.9-990721.tar.xz ee-gcc2.9-991111.tar.xz ee-gcc2.9-991111a.tar.xz \
         ee-gcc2.9-991111-01.tar.xz ee-gcc2.95.2-273a.tar.gz ee-gcc2.95.3-114.tar.gz \
         ee-gcc2.95.3-136.tar.gz ee-gcc2.96.tar.xz; do
    name=${c%.tar.*}
    [ -d "$name" ] && continue
    mkdir "$name"
    curl -sSL "$BASE/$c" | tar -xJf - -C "$name" 2>/dev/null || curl -sSL "$BASE/$c" | tar -xzf - -C "$name"
done
