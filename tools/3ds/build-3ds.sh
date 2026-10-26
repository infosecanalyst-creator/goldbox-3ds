#!/bin/bash
# Called by build-3ds.bat (devkitPro msys2 bash) or Docker: builds goldbox.3dsx.
export DEVKITPRO=${DEVKITPRO:-/opt/devkitpro}
export DEVKITARM=${DEVKITARM:-$DEVKITPRO/devkitARM}
export PATH="$DEVKITPRO/tools/bin:$DEVKITARM/bin:$PATH"
cd "$(dirname "$0")/../.." || exit 1
echo "DEVKITPRO=$DEVKITPRO"
ls "$DEVKITARM/bin/arm-none-eabi-g++" "$DEVKITPRO/libctru/lib" >/dev/null || { echo "devkitARM/libctru missing (pacman -S 3ds-dev)"; exit 2; }
make -f Makefile.3ds -j8
