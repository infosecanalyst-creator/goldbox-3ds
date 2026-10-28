#!/bin/bash
# Called by build-3ds.bat (devkitPro msys2 bash) or Docker: builds goldbox.3dsx.
# make cannot handle spaces in paths (e.g. ".../Desktop/Gold Box/..."), so the
# sources are mirrored to a space-free folder, built there, and the .3dsx is
# copied back.
export DEVKITPRO=${DEVKITPRO:-/opt/devkitpro}
export DEVKITARM=${DEVKITARM:-$DEVKITPRO/devkitARM}
export PATH="$DEVKITPRO/tools/bin:$DEVKITARM/bin:$PATH"
SRC="$(cd "$(dirname "$0")/../.." && pwd)"
echo "DEVKITPRO=$DEVKITPRO"
echo "source: $SRC"
ls "$DEVKITARM/bin/arm-none-eabi-g++" "$DEVKITPRO/libctru/lib" >/dev/null || { echo "devkitARM/libctru missing (pacman -S 3ds-dev)"; exit 2; }

BUILD="${GOLDBOX_BUILD_DIR:-$HOME/goldbox-3ds-build}"
case "$BUILD" in *" "*) BUILD=/tmp/goldbox-3ds-build;; esac
mkdir -p "$BUILD"
# mirror sources (only what the 3DS build needs); keep object files between runs
for d in src assets; do
  mkdir -p "$BUILD/$d"
  cp -ru "$SRC/$d/." "$BUILD/$d/"
done
cp -u "$SRC/Makefile.3ds" "$BUILD/"
echo "building in: $BUILD"
cd "$BUILD" || exit 1
make -f Makefile.3ds -j8 || exit 1
cp -f goldbox.3dsx "$SRC/goldbox.3dsx"
cp -f goldbox.elf "$SRC/goldbox.elf" 2>/dev/null  # symbols, for reading 3DS crash dumps
echo "copied goldbox.3dsx to $SRC"
