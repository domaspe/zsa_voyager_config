# How the firmware works

Supplement to `LAYOUT.md`, which says what the keys do. This says how and why, for whoever changes it next.

Built against ZSA's QMK fork, branch `firmware25`. Source is `zsa_voyager_domas_source/`.

## Mac mode

One variable, `mac_mode`, decides what four keys send. `LT2` and `RT1` send Ctrl or Cmd. `L4-2` and `R4-5` send the Windows key or Cmd. Seven more keycodes read it to pick between a Windows and a Mac shortcut.

Three things set it.

- **Plug-in.** QMK's OS Detection watches how the computer sets up the USB connection. Windows, macOS and Linux each do it differently. `process_detected_host_os_user` turns Mac mode on for `OS_MACOS` and `OS_IOS`, off for `OS_WINDOWS` and `OS_LINUX`.
- **`OS_UNSURE`** changes nothing. Behind a dock or a KVM the guess can fail, and a failed guess must not undo a choice you made by hand.
- **`LT1` + `R4-6`** flips it either way.

It is never written to permanent storage. Every plug-in decides again from scratch. That is deliberate: a stale saved value is worse than a fresh guess, because you cannot see it.

You can see the current value on the keys it changes. The four mode-dependent holds, `LT2`, `RT1`, `L4-2` and `R4-5`, are pink in Windows mode and white in Mac mode, and the Mac mode key on layer 1 `R4-6` wears the same pair while that layer is held. `mode_keys` lists the five keycodes; `set_mode_keys_glow` scans the held layer for them and paints each one white when Mac mode is on, over the colour `glowmap` gives it. They are found by keycode, not position, so moving one in the keymap moves its light with it, and a transparent cell on a layer is never painted.

Changing mode releases whatever the two thumbs are holding and cancels an app switch in progress. Without that, flipping mode mid-chord would leave a modifier stuck down.

## Why QMK's own Ctrl/Cmd swap is not used

QMK has `CG_SWAP`, which trades Ctrl and Cmd across a whole side of the keyboard. It cannot express this layout.

`LT2` needs Ctrl on Windows and Cmd on a Mac. `L4-1` needs Ctrl on both. They are on the same hand, so one flag has to serve both, and no setting of it is right for both. Exempting `L4-1` would take custom code anyway, and then the swap saves nothing.

So there is no swap. Only two keys differ per system, `LT2` and `RT1`, and those two are custom. Every other modifier is a plain QMK mod-tap that means exactly what it says in the keymap.

## Custom keycodes

Six of them are nothing but a choice between two keystrokes, so they live in one table, `os_keycodes`, rather than six copies of the same branch:

| Keycode       | Position       | Windows      | Mac          |
| ------------- | -------------- | ------------ | ------------ |
| `WORD_LEFT`   | layer 2 `R4-2` | Ctrl+Left    | Option+Left  |
| `WORD_RIGHT`  | layer 2 `R4-4` | Ctrl+Right   | Option+Right |
| `LINE_START`  | layer 2 `R2-2` | Home         | Cmd+Left     |
| `LINE_END`    | layer 2 `R2-4` | End          | Cmd+Right    |
| `DELETE_WORD` | layer 2 `R4-6` | Ctrl+Bksp    | Option+Bksp  |
| `PASTE_PLAIN` | layer 1 `L4-5` | Ctrl+Shift+V | Cmd+Shift+V  |

Press registers the chosen keystroke and release lets it go, so holding one repeats. Which keystroke went down is stored per key, so flipping mode while one is held still releases the right thing.

`DELETE_LINE`, layer 2 `R4-5`, is the seventh and is not in the table because it is two keystrokes. Windows has no delete-to-line-start keystroke, so on both systems it taps Shift plus the `LINE_START` keystroke of the current mode, then Backspace: select to the line start, delete the selection. It fires on press only, so holding it does not repeat.

The rest:

| Keycode               | Position               | Behaviour                                                   |
| --------------------- | ---------------------- | ----------------------------------------------------------- |
| `APP_CMD_L`           | `LT2`                  | Tap Escape. Hold Ctrl, or Cmd in Mac mode.                  |
| `APP_CMD_R`           | `RT1`                  | Tap Space. Hold Ctrl, or Cmd in Mac mode.                   |
| `SWITCH_TAB`          | `L2-1`                 | Tab, plus the Alt trade described below.                    |
| `NEXT_TAB`, `PREV_TAB`| layer 1 `L2-1`, `L1-1` | Ctrl+Tab, Ctrl+Shift+Tab. Plain QMK keycodes, named so the grid reads. |
| `MAC_TOGGLE`          | layer 1 `R4-6`         | Flips Mac mode. Pink in Windows mode, white in Mac mode, like the four keys it flips. |
| `BACKSLASH_ENTER`     | layer 1 `L4-6`         | Sends `\` then Enter.                                       |
| `NUM5_CLICK`          | `L1-6`                 | Tap `5`. Hold left mouse button.                            |

`APP_CMD_L` and `APP_CMD_R` are written as `LT(0, ...)`. Layer 0 is the base layer, so the hold does nothing by itself and `process_record_user` supplies the modifier, reading `record->tap.count` to tell a tap from a hold. Which modifier it registered is stored, and release lets go of that stored one rather than recomputing it, so a mode change between press and release cannot strand a key down.

`BACKSLASH_ENTER` replaces two identical Oryx macros, `ST_MACRO_0` and `ST_MACRO_1`, which sent the same thing.

## Alt+Tab on Windows

App switching is the same fingers on both systems: hold `LT2`, tap `L2-1` as many times as you like.

On a Mac that needs no help. The thumb is already holding Cmd, so `SWITCH_TAB` sends a plain Tab and the result is a real Cmd+Tab.

On Windows the thumb holds Ctrl and the switcher wants Alt, so `SWITCH_TAB` trades them:

1. First tap, with the thumb down and Mac mode off: release Ctrl, press Alt, remember that the trade is active.
2. Send Tab. Release of the key sends only the Tab release, so Alt stays down.
3. Later taps see the trade already active and just send Tab. The window list stays open and cycles.
4. When the thumb lifts, release Alt and forget the trade.

Alt is down for exactly as long as the thumb is, which is why this is the real switcher rather than an imitation. You can pause as long as you want, and arrow keys work inside the list.

The obvious alternative is a timer: one key that presses Alt, taps Tab, and drops Alt after a second of quiet. That was rejected. A timer cannot tell a pause from a finish, so thinking for two seconds closes the switcher on whatever happened to be highlighted.

## Tap-hold tuning

Most keys do one thing on a tap and another on a hold. Deciding which you meant is the hard part, and three QMK features split the work.

**Flow Tap** (`FLOW_TAP_TERM 150`) forces the tap when a key goes down within 150 ms of the previous key. During fast typing a modifier cannot appear at all, whatever your fingers do. This is what keeps `xa` from becoming Alt+A.

**Chordal Hold** (`CHORDAL_HOLD`) forces the tap when the other key is on the same hand, on the reasoning that a same-hand pair during typing is a roll, not a chord.

**Permissive Hold** (`PERMISSIVE_HOLD`) allows the hold as soon as the other key has gone down and come back up. A real chord therefore does not wait out the tapping term, so shortcuts stay instant even where the term is long.

### Which keys are exempt, and why

Chordal Hold's same-hand rule is wrong for this layout, because the layout is deliberately one-handed. Cmd+C is the left thumb plus the left middle finger. Win+D is the left pinky plus the left middle finger. Under the plain rule both would type letters.

So `chordal_hold_layout` marks every modifier `*`, which switches the rule off for that key and hands the decision to Permissive Hold:

```
L L L L L *      R R R R R R
L L L L L L      R R R R R R
L L L L L L      R R R R R R
L * * L L L      R R R * * *
          * *      * *
```

`R3-6` is the exception. It stays `R`, so the rule still applies to it. That is what makes a right-hand roll after the right Shift type an apostrophe instead of a capital, which is what you want, because right-hand capitals come from the left Shift.

### Per-key settings

One table, `tap_holds`, feeds three callbacks. Anything not listed takes the defaults: term 150, hold-on-other-press off, retro tap on.

| Position | Keycode             | Term | Hold on other press | Retro tap |
| -------- | ------------------- | ---- | ------------------- | --------- |
| `L4-2`   | `LGUI_T(KC_Z)`      | 200  | no                  | no        |
| `R4-5`   | `RGUI_T(KC_SLASH)`  | 200  | no                  | no        |
| `L4-3`   | `LALT_T(KC_X)`      | 200  | no                  | no        |
| `R4-4`   | `RALT_T(KC_DOT)`    | 200  | no                  | no        |
| `R4-6`   | `RCTL_T(KC_DELETE)` | 200  | no                  | yes       |
| `L1-6`   | `NUM5_CLICK`        | 200  | no                  | no        |
| `R3-6`   | `RSFT_T(KC_QUOTE)`  | 150  | yes                 | yes       |

**Why 200 on the bottom row.** These tap characters you type all day. A longer boundary keeps a slow keystroke a character. Flow Tap covers you inside a burst, but after a pause the term is the only guard, and `./` or `/usr` typed after a pause would otherwise send Alt or the Windows key plus a letter. The longer term costs nothing, because Permissive Hold does not wait for it.

**Why retro tap is off on `L4-2` and `R4-5`.** These are the Windows key, and the Start menu needs a bare press and release with nothing in between. Retro tap would add a stray `Z` or `/` on top of it.

**Why retro tap is off on the two Alt keys.** On Windows a bare Alt press moves focus to the menu bar. A character arriving straight after it could pick a menu item.

**Why hold-on-other-press is on for `R3-6` only.** It makes a capital land the moment the letter goes down, whatever order you lift the two keys. It is safe there because Chordal Hold still catches right-hand rolls and the apostrophe is not a Flow Tap key. On any Space key the same setting would be a disaster: "a you" would become "a You", because Space is a left key and half the letters after it are right-hand ones.

**Why `L3-1` is a plain Shift.** For the same reason. Shift and Space cannot share a key and both win. Space therefore lives only on `RT1`, and `L3-1` has no tap at all, which makes Shift plus a letter a capital at any speed, either hand, any release order.

### Flow Tap coverage

`is_flow_tap_key` takes QMK's stock set — letters, comma, period, semicolon, slash, space — and adds the number row and `NUM5_CLICK`, so rolling digits cannot fire a mouse click. It keeps the stock guard that switches Flow Tap off once Ctrl, Cmd or Alt is already held, which is what lets you chain a second key into a shortcut.

Which keys this protects follows from their tap: `L4-2`, `R4-5`, `L4-3`, `R4-4`, `RT1` and `L1-6` are covered. `LT2`, `LT1`, `RT2`, `R3-6` and `R4-6` are not, because Escape, Enter, Backspace, apostrophe and Delete are not typing keys.

## Lighting

Every key has a colour per layer. `glowmap` holds them as a `LAYOUT_voyager` grid of colour numbers, one grid per layer, in the same shape as `keymaps`. `glow_palette` turns a number into HSV, and `set_layer_color` walks the matrix, finds each key's LED through `g_led_config.matrix_co` and paints it. `GLOW_OFF` is 0 because the macro fills the unused matrix cells with `KC_NO`, which is also 0.

The Oryx export kept colours as a flat list in LED order, which is not key order. The grid replaces it so that a colour sits where its key sits, and a layout change cannot point a colour at the wrong key.

### Choosing colours

`LAYOUT.md` **Colours** says what each colour means on each layer. The rules behind it:

- **Blue is base-only.** Every key that only types is blue on the base layer, and nothing on a layer is blue. A blue key therefore proves no layer is held. This is why F1 is lime rather than blue: on a layer the legends stop helping, and a blue top row would look like the number row.
- **Keys that depend on the mode show the mode.** The four pink holds on base and the Mac mode key on layer 1 are pink in Windows mode and white in Mac mode. Nothing else changes colour with the mode; `L4-1` is plain orange because Ctrl is Ctrl on both systems.
- **On base, anything not blue is a hold.** Warm colours hold a modifier: orange a fixed one (Shift, Ctrl, Alt), pink one that changes with the system (Ctrl or Cmd on `LT2` and `RT1`, Win or Cmd on `L4-2` and `R4-5`), red the mouse button. Green holds a layer. The outer columns read as a mirrored pair through this alone: blue in the top two rows, orange in the bottom two, on both sides.
- **Four colours are constant.** White is a key that changes the board itself, lime is the F keys, green is the layer thumbs, red is the mouse wherever the layer has a mouse key. Layer 1 has no mouse key, so red is `\ |` there.
- **Layer 2 colours by unit.** Purple moves by a character, orange by a word, cyan by a line, pink by a page. Del word is orange and Del line cyan for the same reason. Position says whether a key moves or deletes.
- **Layer 1 colours by pair.** Each symbol pair has its own colour. Prev tab, Next tab, Paste plain and `\` + Enter share pink because all four send a shortcut or a sequence rather than a character.
- **Touching groups differ.** Two keys next to each other that do different kinds of thing get different colours. Beyond that, hues that are neighbours on the wheel (42° apart) stay off touching keys wherever there is a choice. The one exception is PgDn (pink, `R3-5`) beside Right (purple, `R3-4`).
- **Anything else may be reused.** Orange is fixed modifiers on base, `[ ]` on layer 1, words on layer 2. That is fine because they never appear together. The meaning table in `LAYOUT.md` is per layer for this reason.

Eight hues, each at least 42° from every other on the hue wheel, so any two can sit side by side without a further rule. In QMK's 0–255 hue scale: red 254, orange 28, lime 60, green 92, cyan 128, blue 164, purple 194, pink 224. The smallest gap is 30 units, which is 42°. Saturation and value are always 255. `hsv_to_rgb_with_value` scales every key by the global brightness, which `L2-5` and `L2-6` on layer 2 set and the board remembers, so a per-key shade would fight the brightness setting and fade first when it is low.

White means Mac mode on the five mode keys, and on layer 2 marks the six lighting keys `L2-5`, `L2-6`, `L3-5`, `L3-6`, `L4-5`, `L4-6`, which change the board itself. No other key is white, so a white key on base always means Mac mode is on.

`TOGGLE_LAYER_COLOR` on layer 2 `L4-6` flips `keyboard_config.disable_layer_led`. With it on, `rgb_matrix_indicators_user` skips the grid and the plain RGB effect shows; the mode keys are still painted white in Mac mode.

## Known limits

- **Five combos need both hands.** They are listed at the end of `LAYOUT.md` with the reason for each.
- **A modifier on `L4-2`, `R4-5`, `L4-3` or `R4-4` is unavailable for 150 ms after a keystroke**, because Flow Tap is suppressing it. Shortcuts from those four need a short pause first. The `LT2` and `LT1` thumbs have no such delay, and most shortcuts come from `LT2`.
- **Escape and Cmd share `LT2`.** Any Mac shortcut that needs both, Force Quit being the one that matters, has to take Cmd from the other thumb.
- **OS Detection is a guess.** If it lands wrong through a dock or a KVM, use `LT1` + `R4-6`. If it lands wrong every time, add `#define OS_DETECTION_KEYBOARD_RESET` to `config.h`, which makes the keyboard restart when the USB connection is set up again and usually fixes a stale reading. It costs a visible restart on every plug-in, so do not add it without the problem.

## Build

Two commands, every time:

```
~/.venvs/qmk/bin/qmk compile -kb zsa/voyager -km domas
cp ~/projects/keyboard/qmk_firmware/zsa_voyager_domas.bin \
   '/mnt/c/Users/DomasPetkevičius/Documents/Projects/zsa_voyager_domas.bin'
```

The first writes `~/projects/keyboard/qmk_firmware/zsa_voyager_domas.bin`. The second puts it where the flasher can open it; see **Flash** below for why.

`qmk compile` runs from any folder. It finds QMK through `user.qmk_home` in `~/.config/qmk/qmk.ini`. `-kb` names the folder under `keyboards/`, `-km` the folder under `keymaps/`.

A clean build prints `Size after:` and copies the `.bin` out. Errors in `keymap.c` are **not** reported under that name: QMK pulls the file into `quantum/keymap_introspection.c` and compiles it there, so that is the filename in the message.

### Setting up the build, once

QMK's own code does most of the work, so both it and the compiler have to be on the machine.

**QMK's code.** ZSA's version, not the original — the original lacks the modules this `keymap.c` needs.

```
git clone --depth 1 --shallow-submodules --recurse-submodules -b firmware25 \
  https://github.com/zsa/qmk_firmware.git ~/projects/keyboard/qmk_firmware
```

**This layout, placed inside it.** A link, not a copy, so there is only ever one set of files to edit:

```
ln -s ~/projects/keyboard/zsa_voyager_xB6Jx_GGRD4w_domas_source/zsa_voyager_domas_source \
      ~/projects/keyboard/qmk_firmware/keyboards/zsa/voyager/keymaps/domas
```

The link's name, `domas`, is what `-km domas` refers to.

**The compiler.** The Voyager runs an STM32 chip, so it needs a compiler that produces ARM code, plus a small C library for a machine with no operating system. QMK ships the script that installs them; run it from the clone:

```
~/projects/keyboard/qmk_firmware/util/qmk_install.sh -y
```

That is the script `qmk setup` calls. On Ubuntu it runs `util/install/debian.sh`. Two of its steps are expected noise here and neither breaks anything: a warning that WSL cannot reach USB devices, which does not matter because flashing happens through a Windows program, and a closing `pip install --user` that fails on Ubuntu 24.04 with `error: externally-managed-environment`, after apt has already finished and for packages the virtual environment below already holds.

Do not run `qmk setup` itself. It downloads the original QMK into `~/qmk_firmware` and repoints `user.qmk_home` at it, which loses the ZSA setup.

By hand, the same two packages:

```
sudo apt install -y gcc-arm-none-eabi libnewlib-arm-none-eabi
```

`libnewlib-arm-none-eabi` is the C library. Ubuntu lists it only as a recommendation of the compiler, not a dependency, so apt normally installs it but a machine set to skip recommendations will not. Without it the build stops on `fatal error: stdint.h: No such file or directory`, which reads like a broken compiler rather than a missing library. `binutils-arm-none-eabi` needs no mention; the compiler package depends on it.

Do not try Homebrew. It has `arm-none-eabi-gcc` but no `newlib` formula, so its compiler cannot build for a machine with no operating system and fails in exactly that way.

**QMK's command line**, in its own virtual environment so it does not depend on whichever Python comes first on `PATH`:

```
python3 -m venv ~/.venvs/qmk
~/.venvs/qmk/bin/pip install qmk
~/.venvs/qmk/bin/qmk config user.qmk_home=~/projects/keyboard/qmk_firmware
```

## Flash

Zapp does the flashing:

```
/mnt/c/Users/DomasPetkevičius/Documents/Projects/zapp.exe
```

It is a command line program, not a window-based one. Run `zapp.exe --help` to see its two commands, `flash` and `update`. Only `flash` is used here; `update` fetches from Oryx and would overwrite this firmware with generated code.

It runs as a Windows program even when started from WSL, so it reaches the keyboard over USB. WSL's own lack of USB access does not apply.

**A Windows program cannot open a WSL path** such as `/home/domas/...`. That is why the build ends with a copy into a Windows folder. Give Zapp a plain filename and run it from that folder:

```
cd '/mnt/c/Users/DomasPetkevičius/Documents/Projects' && ./zapp.exe flash zsa_voyager_domas.bin
```

### Putting the board into flash mode

Zapp waits for the keyboard to appear in flash mode; it does not put it there. Its messages for this are `Waiting for keyboard in bootloader mode...`, `Failed to detect bootloader` and `Timeout waiting for bootloader`.

This layout has no key that enters flash mode. `QK_BOOT` appears nowhere in `keymap.c`, and was absent from the Oryx original too. So the reset button on the board is the only way in. Find it before flashing, not while the keyboard is dead.

### Going back

A bad flash is recoverable. The chip holds two separate programs: the one that receives new firmware, which ZSA calls Ignition, and the keymap firmware. Flashing replaces the second and never the first, so firmware that crashes on startup cannot stop you writing over it. Zapp names both states in its device list, `Voyager (Ignition STM32)` and `Keyboard in Reset Mode (STM32 DFU)`.

`zsa_voyager_xB6Jx.bin` in this repo is the build that came before this rewrite, kept for exactly this. `zsa_voyager_xB6Jx.bin.md5` holds its checksum. That file is the bare value with no filename, so `md5sum -c` cannot read it; compare the two by hand:

```
md5sum zsa_voyager_xB6Jx.bin
cat zsa_voyager_xB6Jx.bin.md5
```

Both must read `306743d7d1156ef2fc826474ddcb92eb`.

To go back, copy it to the Windows folder and flash that instead:

```
cp zsa_voyager_xB6Jx.bin '/mnt/c/Users/DomasPetkevičius/Documents/Projects/zsa_voyager_xB6Jx_RESTORE.bin'
cd '/mnt/c/Users/DomasPetkevičius/Documents/Projects' && ./zapp.exe flash zsa_voyager_xB6Jx_RESTORE.bin
```

Keep that file until a new build has been flashed and checked.

## Checking after a flash

A compile proves the code builds. Only typing proves it works.

**Mode**

1. Plug into Windows. `LT2`, `RT1`, `L4-2` and `R4-5` are pink.
2. Plug into a Mac. The same four turn white within about a second. `L4-1` stays orange.
3. `LT1` + `R4-6` flips the four lights and the mode. Again flips it back. While `LT1` is held, `R4-6` is pink in Windows mode and white in Mac mode.

**Same finger, both systems**

4. `LT2` + `L4-4` copies, `LT2` + `L4-5` pastes, `LT2` + `L3-3` saves.
5. `L4-1` + `L4-4` cancels a running command in a terminal. On the Mac, check it does not copy.
6. `R4-6` + `R2-2` clears the line in a terminal.
7. `RT1` + `R3-4` opens the address bar. `RT1` + `R1-6` and `R2-6` zoom out and in.

**Switching**

8. Hold `LT2`, tap `L2-1` three times, pause with the thumb still down, then release. The list must stay open through the pause and land on the third window.
9. Release the thumb and type a letter. It must be plain, with no Alt or Ctrl left down.
10. `LT1` + `L2-1` moves to the next browser tab, `LT1` + `L1-1` and `LT1` + `L3-1` + `L2-1` to the previous one.

**Movement**

11. `RT2` + `R3-2` and `R3-4` move one character, `RT2` + `R4-2` and `R4-4` one word.
12. `RT2` + `R2-2` and `R2-4` reach line start and end. On the Mac test in a text field, not only in a terminal.
13. `RT2` + `R3-6` + `R4-2` selects the previous word with one hand.
14. `RT2` + `R4-6` deletes the word before the cursor. `RT2` + `R4-5` deletes back to the line start, on both systems and in a browser text field on the Mac.

**Typing safety**

15. Type `xa`, `za`, `./` and `/u` fast. Each must give the characters, never a modifier chord.
16. Type a sentence at full speed with capitals from `L3-1` and from `R3-6`. Every capital must appear.
17. Hold `R3-6` and type a right-hand letter. It must give an apostrophe and the letter, not a capital.
18. Hold `L4-2` alone for half a second and release. On Windows the Start menu opens and no `z` is typed.
19. Tap `L4-2` quickly. It types `z`.
20. Hold `LT2` alone and release. It sends Escape.
21. Hold `RT1` slowly and release. It sends Space.

**Colours**

22. Base layer: blue everywhere except orange on `L3-1`, `R3-6`, `L4-1`, `R4-6`, `L4-3`, `R4-4`; pink on `L4-2`, `R4-5`, `LT2`, `RT1`; green on `LT1`, `RT2`; red on `L1-6`.
23. Hold `LT1`. The board matches the layer 1 grid in `LAYOUT.md` **Colours**: lime top row, four symbol pairs in four colours, nothing blue, nothing lit on the right hand except F6–F12 and `R4-6`, everything marked `_` dark. `LT1` + `R2-1` types `y` and does not scroll.
24. Hold `RT2`. Same for the layer 2 grid: six white keys on the left hand, and PgDn (`R3-5`) tellable from Right (`R3-4`) at a glance.
25. In Mac mode `LT2`, `RT1`, `L4-2` and `R4-5` are white on the base layer, `R4-6` is white on layer 1, and nothing else changes.
26. Judge orange against lime, and lime against green, by eye. If two read alike, move a hue in `glow_palette` and keep every gap at 30 units or more.
