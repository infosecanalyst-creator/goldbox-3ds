#!/bin/bash
# Compile every core source for the 3DS CPU with Ubuntu's arm-none-eabi toolchain
# (newlib, like devkitARM) to catch portability errors before the real build.
cd "$(dirname "$0")/../.."
ok=0; bad=0
for f in $(find src/cs src/Classes src/engine src/host/common -name '*.cpp'); do
  if arm-none-eabi-g++ -std=gnu++17 -march=armv6k -mtune=mpcore -mfloat-abi=hard -mfpu=vfp -O0 -fsyntax-only -fwrapv \
     -D__3DS__ -D_GNU_SOURCE -Itools/armcheck/inc -Isrc -Wno-psabi "$f" > /tmp/armcheck-$(basename $f).log 2>&1; then ok=$((ok+1)); else bad=$((bad+1)); echo "FAIL $f"; fi
done
echo "ok=$ok bad=$bad"
