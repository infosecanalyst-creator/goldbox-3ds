# goldbox-3ds

A native, dual-screen engine for SSI's *Gold Box* AD&D games, aimed at the
New Nintendo 3DS, with SDL2 desktop and headless builds for development.

It starts from [coab](https://github.com/simeonpilgrim/coab), Simeon Pilgrim's
C# reimplementation of **Curse of the Azure Bonds**, translated line-for-line
into C++17 so that it can be verified against coab and then extended.

**No game data is included.** You need your own copy (e.g. GOG's
*Forgotten Realms: The Archives – Collection Two*); point the game at the folder
containing the `.DAX` files.

## Status

| Phase | Goal | State |
|---|---|---|
| 1 | coab reference build running against GOG data | done |
| 2 | C++ translation + SDL2/headless builds reach the title screen | in progress |
| 2b | Differential testing vs coab (same seed, same inputs, compare state) | planned |
| 3 | Nintendo 3DS backend (citro2d, ndsp, dual-screen UI, Wi-Fi deploy) | planned |
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
assets/sounds/ coab's sound samples
```

## Building (desktop)

```sh
cmake -B build && cmake --build build -j
./build/goldbox --data /path/to/curse_of_the_azure_bonds
```

Windows binaries are cross-compiled with MinGW (`cmake/mingw-w64.cmake`).

## License

BSD 3-clause, same as coab. See `LICENSE`.
