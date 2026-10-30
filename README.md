# goldbox-3ds

> **Bring your own game files.** This repository contains no game data, art,
> sound or text from the original games. You need your own legally obtained
> copy of the game (e.g. GOG's *Forgotten Realms: The Archives – Collection
> Two*) to play.
>
> Not affiliated with or endorsed by Wizards of the Coast, SSI, Ubisoft or
> Nintendo; names are trademarks of their owners.
>
> The engine core is derived from [coab](https://github.com/simeonpilgrim/coab).
> This project publishes **source code only — no prebuilt binaries**.

A native, dual-screen engine for SSI's *Gold Box* AD&D games, aimed at the
New Nintendo 3DS, with SDL2 desktop and headless builds for development.

It starts from [coab](https://github.com/simeonpilgrim/coab), Simeon Pilgrim's
C# reimplementation of **Curse of the Azure Bonds**, translated line-for-line
into C++17 so that it can be verified against coab and then extended.

Point the game at the folder containing your own `.DAX` files (on the 3DS:
`sdmc:/3ds/goldbox/cotab/`).

### Sound (optional)

No sound samples are included. If you have your own copy of the sound samples
coab uses (`death.wav`, `hit.wav`, `magic.wav`, `magic_hit.wav`, `miss.wav`,
`missle.wav`, `sound_5.wav`, `sound_10.wav`, `start_sound.wav`, `step.wav`),
put them in:

* 3DS: `sdmc:/3ds/goldbox/sounds/` (`tools/3ds/deploy-3ds.bat` uploads them from
  `../data/sounds/` if that folder exists)
* desktop: a `sounds/` folder next to the `goldbox` executable, or pass
  `--sounds DIR`

Any missing sample is simply silent; the game runs fine without them.

## Status

| Phase | Goal | State |
|---|---|---|
| 1 | coab reference build running against GOG data | done |
| 2 | C++ translation + SDL2/headless builds reach the title screen | done (plays the demo through combat) |
| 2b | Differential testing vs coab (same seed, same inputs, compare state) | demo runs byte-identical to coab (3 seeds x 20,000 rolls); more scripts to add |
| 3 | Nintendo 3DS backend (dual-screen UI, touch menus, ndsp, Wi-Fi deploy) | builds (goldbox.3dsx); first on-device test pending |
| 4 | Other Gold Box games via per-game engine profiles | planned |

## Layout

```
src/cs/        C#-compatibility runtime (Array, List, String, Format, bit-exact Random, IO)
src/Classes/   translation of coab/Classes (data model, DAX loading, display)
src/engine/    translation of coab/engine (game logic, ECL interpreter, combat)
src/Logging/   coab's logger/config
src/platform/  interface the engine needs from a host (frame, sound, sleep)
src/host/      hosts: pc (SDL2), headless (scripted test runner), 3ds (later)
docs/          TRANSLATION.md - the rules the translation follows
```

## Building (desktop)

```sh
cmake -B build && cmake --build build -j
./build/goldbox --data /path/to/curse_of_the_azure_bonds
```

Windows builds can be cross-compiled with MinGW (`cmake/mingw-w64.cmake`).

## Building (3DS)

`make -f Makefile.3ds` with devkitARM + libctru (or `tools/3ds/build-3ds.bat`,
which falls back to the devkitpro/devkitarm Docker image). Install over Wi-Fi
with ftpd running on the 3DS:

```
tools\3ds\deploy-3ds.bat <3ds-ip>      (or set GOLDBOX_3DS_IP)
tools\3ds\3dslink.bat <3ds-ip>         (Homebrew Launcher netloader, press Y)
```

## License

This project's own code is released under the BSD 3-Clause licence (see
`LICENSE`).

The engine core is a C++ translation of coab, Copyright (c) 2010 Simeon
Pilgrim. coab's GitHub repository has no top-level licence file; the licence it
does distribute is a BSD 3-Clause text shipped with its Windows installer
(`Installer/CotAB License.rtf` in the coab repository). That notice is
reproduced in `LICENSE`, as its terms require. coab is itself a reimplementation
of SSI's DOS game made from a disassembly; neither coab's licence nor this one
grants any rights in the original game, its data or its trademarks.

The built-in pixel font is Press Start 2P, under the SIL Open Font License 1.1
(`docs/licenses/OFL-PressStart2P.txt`).
