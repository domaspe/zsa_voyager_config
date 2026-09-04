# Working in this repo

Firmware source for a ZSA Voyager keyboard, one layout serving both Windows and macOS. It started as an Oryx export and has since been hand-edited well past anything Oryx can represent.

Read `LAYOUT.md` for what the keys do and `FIRMWARE.md` for how it works.

## The tree

```
zsa_voyager_domas_source/
  keymap.c        the layout and all custom behaviour
  config.h        tap-hold timing, RGB timeout, serial number
  rules.mk        which QMK features are compiled in
  keymap.json     REQUIRED. Switches on ZSA's three modules. Holds no layout.
  keymap.c.orig   the Oryx original, kept for reference
LAYOUT.md         key reference for a person
FIRMWARE.md       how and why, for whoever changes it
zsa_voyager_xB6Jx.bin   an older build, kept as something to go back to
```

## Hard rules

**A fresh Oryx download destroys this work.** Downloading the layout again from configure.zsa.io overwrites `keymap.c`, `config.h` and `rules.mk` with generated code, losing OS detection, the tap-hold tuning and every custom keycode. Never regenerate from Oryx without saying so first and getting an answer.

**`keymap.c`, `LAYOUT.md` and `FIRMWARE.md` change together.** A key that moves in one moves in all three. A reader who trusts a stale `LAYOUT.md` will mistype for a week before working out why. Colour is part of this: a key that changes what it does also changes its colour in `glowmap`, in the colour grids of `LAYOUT.md` **Colours** and in that section's meaning table, checked against its neighbours by the rules in `FIRMWARE.md` **Lighting**.

**`keymap.json` is not the layout and must stay.** It holds nothing but ZSA's module list (`zsa/oryx`, `zsa/navigator_trackpad`, `zsa/defaults`). Those modules supply `ZSA_SAFE_RANGE`, `rawhid_state` and `TOGGLE_LAYER_COLOR`, which `keymap.c` uses, so deleting the file breaks the build. QMK reads it and then pulls in `keymap.c` as well; both are used together (`builddefs/build_keyboard.mk`, the `keymap.json` block).

**Colours come only from `glow_palette`.** No one-off HSV value anywhere else. A new colour is added to `enum glow`, `glow_palette`, `LAYOUT.md` and `FIRMWARE.md` in the same change, and has to sit at least 42° from every existing hue so it can touch any of them.

**Do not touch** `SERIAL_NUMBER` in `config.h`. The serial number identifies this board to ZSA's tools.

## Position codes

Every grid in both documents is in position codes, and `FIRMWARE.md` uses nothing else. The shortcut tables in `LAYOUT.md` are the exception: they lead with what is printed on the key, because that is what you look at while finding a combo, and keep the position code in a column beside it. A position code survives a base layer change; a printed legend does not, so anything that moves a key has to correct the legends in those tables as well.

Hand, then row 1 at the top to row 4 at the bottom, then column 1 to 6 left to right as you look at the board. `L4-1` is the bottom-left corner. `R2-5` is the right hand, second row, fifth column. Thumbs are `LT1` and `LT2` on the left, `RT1` and `RT2` on the right, numbered outward to inward on the left and inward to outward on the right, so the inner pair is `LT2` and `RT1`.

Columns map to fingers. On the left, column 1 is the outer pinky through column 6 the inner index. The right hand mirrors it, so column 1 is the inner index and column 6 the outer pinky.

## Code conventions

The Oryx original is machine-written. Do not copy its style.

- **Comment the non-obvious reason, not the mechanics.** The comment about reapplying modifiers to mouse and media keys earns its place: the code cannot show why it is needed. Names carry the rest.
- **Tables, not repeated switch arms.** Anything that varies per key belongs in one `static const` table with a single lookup, so a key is described in one place. `os_keycodes`, `tap_holds` and `glowmap` are the three that exist; extend them rather than adding a branch.
- **QMK's short mod-tap aliases** in the keymap grid: `LGUI_T(KC_Z)`, not `MT(MOD_LGUI, KC_Z)`. The grid stays readable only while the rows stay short.
- **Name a key whose purpose its keycode does not show.** `APP_CMD_L` reads; `LT(0, KC_ESCAPE)` does not.
- **No magic indexes.** Find an LED through `g_led_config.matrix_co` from its matrix position, so a layout change cannot silently point it at the wrong key.

## Verifying a change

The layout can be compiled here. ZSA's fork belongs at `../qmk_firmware` on branch `firmware25`, with this directory linked in at `keyboards/zsa/voyager/keymaps/domas` so QMK sees these files without a second copy. `FIRMWARE.md` has the download, the link and the compiler in full under **Build**, and the flasher, how to enter flash mode and how to go back under **Flash**.

```
~/.venvs/qmk/bin/qmk compile -kb zsa/voyager -km domas
```

If that copy of QMK is not present, say plainly that the change was not compiled. Do not describe a change that was not compiled in words that imply a build passed.

Errors in `keymap.c` are reported against `quantum/keymap_introspection.c`, which pulls it in. Nothing here appears under its own filename.

Flashing needs a Windows path. Zapp is a Windows program and cannot open `/home/...`, so the built `.bin` has to be copied under `/mnt/c/` first. `FIRMWARE.md` gives the two commands.

A compile is not a test. Tap-hold timing, OS detection and the app switcher can only be judged by typing on the board. `FIRMWARE.md` ends with the by-hand checks worth running after a flash.
