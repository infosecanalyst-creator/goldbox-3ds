# 3DS design decisions (apply to every Gold Box game)

Settled while building the Curse of the Azure Bonds profile (Oct 2026). Reuse all
of this for Pool of Radiance, Secret of the Silver Blades, Pools of Darkness,
the Krynn and Savage Frontier games, etc.

## Principles
- Never send anything to the engine but the keys the original game expects.
  Port-only behaviour is behind `gbl::portEnhancements`, off in the coab
  differential harness, so the engine stays byte-identical in the tests.
- Minimise the keyboard: every menu word, hotkey line, list entry, party member,
  yes/no and amount is tappable or on a button. The keyboard opens and closes by
  itself only for real text (names, save names, riddle answers), with RANDOM for
  character names. Amounts use a number pad (ALL / CLEAR / OK / CANCEL).
- Wait for the engine: key repeat and taps hold back while keys are queued
  (`host::PendingKeys`), so movement stops when the button is released.

## Screens
- Exploring and menus: the game on the TOP screen, filling all of it (400x240:
  5/4 across, 6/5 down; no side bars, no touch-only chrome). Text is redrawn
  crisp at 1:1 in the enlarged 10 x 9.6 cells, on whole pixels.
- With the 3D slider down the top screen runs in its 800x240 wide mode
  (`gfxSetWide(true)`, `gfxSet3D(false)`); layouts stay in 400x240 coordinates
  (each layout pixel is two screen columns) and the game picture is drawn into
  all 800 columns (2.5 per game pixel: even 2/3-column pixels instead of 1/2).
  Slider up: back to the two 400x240 eyes for the dungeon stereo.
- The touch screen then shows the game's current choices as big buttons, and while
  exploring an automap (visited squares, walls, doors, facing arrow; saved per
  map) beside the menu words.
- Combat: the game moves to the TOUCH screen for stylus aiming (tap a square
  once to aim, again to Target); combat menu words become the bottom strip; the
  top screen is a combat panel (HP bars, AC, status, whose turn).
- ZL / SELECT / SWAP flips the automatic choice until combat starts or ends.
- Combat swapped to the top: the touch screen's free area is a touchpad (slide
  moves a pointer drawn on the top screen, a short tap = a tap on the game at
  the pointer: aim, then Target). No right button: the game has none.
- Bottom strip: ESC YES NO ENTER KEYS SWAP BOOK (BOOK shows "J.nn" when the game
  mentions a journal entry).

## Text and look
- Touch buttons and keyboard keys click (a very subtle, quiet tick on its own channel) and
  visibly press: they light up, the label sinks 1 px, the raised edge goes away.
- Clear 8x8 font (Press Start 2P, OFL) instead of the game font, case kept.
- Story text appears instantly (no per-character delay); timed message pauses
  are kept.
- Title menu waits on Play (no automatic demo); any button ends the demo.
- Lists highlight their action word (Select) rather than Exit; hotkey menus get a
  host-drawn gold highlight moved by the D-Pad.

## Buttons without the stylus (gold ring)
- Every touch button works with the buttons. A gold ring (2 px outside the
  button) shows what A presses; A gives the same click and press animation.
- Menus and lists: the D-Pad moves the game's own highlight; the ring sits on
  the matching big button (list entry, hotkey line, highlighted menu word -
  the engine publishes the word via `platform::MenuHighlight`, display-only).
- Keyboard / number pad: the D-Pad moves the ring between keys, A presses,
  START = Enter.
- ZR: the ring moves onto the touch-screen buttons (strip + choices) while the
  D-Pad would walk; D-Pad moves it, A presses (then back to walking), B / ZR
  leave. The book: L / R run on across tabs, so every tab is reachable.

## Controls
- A / START = Enter (START on release), B = Esc, X = Yes, Y = Space (Backspace
  while typing).
- D-Pad / Circle Pad: walk and turn; step menu words; move list highlights
  (Home/End); 8-way in combat (diagonals given ~80 ms to form).
- L / R = previous / next party member; in combat L / R are the up diagonals,
  C-Stick left / right the down diagonals; in the Aim menu L/R = previous/next
  target. C-Stick up / down page lists. ZL = swap screens, ZR = button ring.
- Hold START + SELECT for a second to quit.

## Text must fit (checked on every build)
- No text may be cut off or split mid-word: buttons, tabs, headers, help lines,
  dashboards. A word a little too wide for its button is drawn condensed (7 px).
  Labels wrap to two lines on buttons 19+ px tall; tabs use short names; help
  lines stay within 39 characters on the bottom screen (48 on the top, 8x8 font).
- The host logs any text that runs off a screen or any label it had to shorten
  to `sdmc:/3ds/goldbox/user/textfit.log`. Before calling a build done, review
  new screens for fit and pull that log over FTP; fix anything in it.

## 3D
- Subtle, slider-controlled stereo only in the dungeon view: the engine tags
  pixels with depth layers while drawing (sky/floor graded by row, far/mid/near
  walls, pictures, sprites); the host shifts each layer per eye (max ~2.5 px),
  nearest layer wins, gaps show the background. Everything else stays flat.

## The book (manual + journal)
- One viewer for the controls manual (romfs, written for the port) and the
  user's own journal / manual / quick reference, made by OCR from their PDFs into
  `sdmc:/3ds/goldbox/manual/*.txt` (`# Book`, `## Section`, paragraphs). The
  publisher's text is never committed to the repo.
- Standard controls across all of these 3DS ports: Circle Pad / C-Stick scroll the
  text (hold to repeat; the game ignores them while reading); the D-Pad moves
  between the section buttons on the touch screen (A opens; the D-Pad does not
  scroll); L / R previous / next section; tap a tab or section; B or START closes.
- The journal shows its entries as a grid of numbers; when the game text says
  "Journal Entry nn", BOOK becomes "J.nn" and opens that entry.

## Delivery
- Installs over Wi-Fi only: build on the PC, FTP the .3dsx to
  `sdmc:/3ds/goldbox/` with ftpd running; never ask for SD card swaps.
