# Changelog

## 2026-11-21

- Lists with a heading (Pick Race, Pick Class...): the heading is no longer one of the six touch buttons, so the last choice (Human, Ranger) has a button straight away.
- The code is smaller, which leaves more memory free on the console.
- The manual explains the exit save (slot J), what happens when a save fails, where the files and logs live.

## 2026-11-20

- Saving is safer: saves and the automap are written to a temporary file and checked before they replace the old one. "Save failed. Is the SD card full?" is shown when a save fails.
- If the game stops on an error, a GAME STOPPED screen shows it and says your last save is unchanged.
- EXIT GAME does not try to save in the middle of an event (it says NOT SAVED and asks before quitting).
- HOME > Close and pressing HOME save to slot J, like EXIT GAME.
- The detailed coab log is off by default; an empty /3ds/goldbox/debug.txt turns it on.
- Fixed a crash on exit.
- Install path: the program is /3ds/goldbox.3dsx.
- The START menu shows the build version.

## 2026-11-19

- Exit asks first when the slot J save failed.
- The book: drag the stylus to scroll with a little momentum; the section buttons act on a tap.
