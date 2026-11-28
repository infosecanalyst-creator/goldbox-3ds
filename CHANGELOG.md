# Changelog

## 2026-11-27

- Same as the other 3DS ports (cross-port pass). START menu in the house order: RESUME, MANUAL, SAVE GAME, JOURNAL, LEFT HAND, EXIT GAME (SAVE was second and MANUAL third; SAVE is now called SAVE GAME). LEFT HAND is new in the menu: the same switch as LEFT: ON / OFF in the manual.
- The exit question reads "Exit Gold Box?" with YES / NO (the button said "YES, EXIT"); under it "It is saved first (slot J)." or "Not saved here: progress since your last save is lost."
- The manual's footer uses the house words, "Sticks: scroll   D-Pad, L / R: section   B / START: close", on two lines (the 8 px font holds 50 letters across).

## 2026-11-21 (audit fixes)

- The start menu no longer shows eight stray letter buttons (C D M T H V A R) the first time it is drawn, and its four choices stay where they are between taps.
- "Exit to DOS" is now "Exit Game" on the port's buttons and opens the port's own "Exit the game?" question (ring on NO, B cancels). The game's own quit question, when reached another way, starts on No.
- B answers No on a Yes / No question. Questions that quit, drop, overwrite or discard start on No.
- "Reroll stats?": the buttons read REROLL and KEEP THESE, and the highlight starts on KEEP THESE.
- START opens the menu while the keyboard is up too (it used to be Enter and accepted a half-typed name).
- SAVE in the START menu: makes camp and opens the game's save list; grey where the game cannot be saved.
- The exit question is said once: question and buttons on the touch screen, the detail on the top screen.
- Strip keys the game is not asking for are grey; YES / NO are not shown twice.
- Game on the touch screen: the first tap on a small line highlights it, the second picks it. The dashboard marks the highlighted word in gold, says when a list has more entries, and shows the buttons while there is no party.
- The journal marks the entries the game has sent you to (gold corner). Common scan errors in the journal text are cleaned as it is read.
- Start-up check: lists missing game files instead of failing later.
- The left-hand setting says so when the card refuses to save it.
- Manual: shorter section names; the text matches all of the above.

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
