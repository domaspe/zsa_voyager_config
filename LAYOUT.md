# Voyager layout

One layout for Windows and macOS. The keyboard detects which computer it is
plugged into and changes four keys to match. Everything else is the same on
both.

Source of truth is `zsa_voyager_domas_source/keymap.c`. How and why it works is
in `FIRMWARE.md`.

## Position codes

Hand, then row 1 at the top to row 4 at the bottom, then column 1 to 6 left to
right as you look at the board.

```
┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐     ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
│   L1-1    │   L1-2    │   L1-3    │   L1-4    │   L1-5    │   L1-6    │     │   R1-1    │   R1-2    │   R1-3    │   R1-4    │   R1-5    │   R1-6    │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│   L2-1    │   L2-2    │   L2-3    │   L2-4    │   L2-5    │   L2-6    │     │   R2-1    │   R2-2    │   R2-3    │   R2-4    │   R2-5    │   R2-6    │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│   L3-1    │   L3-2    │   L3-3    │   L3-4    │   L3-5    │   L3-6    │     │   R3-1    │   R3-2    │   R3-3    │   R3-4    │   R3-5    │   R3-6    │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│   L4-1    │   L4-2    │   L4-3    │   L4-4    │   L4-5    │   L4-6    │     │   R4-1    │   R4-2    │   R4-3    │   R4-4    │   R4-5    │   R4-6    │
└───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘     └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
                                                 ┌───────────┬───────────┐     ┌───────────┬───────────┐
                                                 │    LT1    │    LT2    │     │    RT1    │    RT2    │
                                                 └───────────┴───────────┘     └───────────┴───────────┘
```

| Column | Left hand   | Right hand  |
| ------ | ----------- | ----------- |
| 1      | outer pinky | inner index |
| 2      | pinky       | index       |
| 3      | ring        | middle      |
| 4      | middle      | ring        |
| 5      | index       | pinky       |
| 6      | inner index | outer pinky |

Thumbs: `LT1` left outer, `LT2` left inner, `RT1` right inner, `RT2` right
outer. The inner pair is `LT2` and `RT1`.

The shortcut tables lead with what is printed on the key and give the position
beside it. The base layer grid below maps one to the other.

## Reading the grids

- `tap/hold` — tap gives the first, hold gives the second.
- `L1`, `L2` — holding opens layer 1 or layer 2.
- `_` — transparent, the key does whatever the base layer says.
- `*` — the hold sends Cmd on a Mac instead. Four keys only, listed in the
  modifier table.

## Base layer

```
┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐     ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
│     `     │     1     │     2     │     3     │     4     │  5/Click  │     │     6     │     7     │     8     │     9     │     0     │     -     │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│    Tab    │     Q     │     W     │     E     │     R     │     T     │     │     Y     │     U     │     I     │     O     │     P     │     =     │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│   Shift   │     A     │     S     │     D     │     F     │     G     │     │     H     │     J     │     K     │     L     │     ;     │  '/Shift  │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│   Ctrl    │  Z/Win*   │   X/Alt   │     C     │     V     │     B     │     │     N     │     M     │     ,     │   ./Alt   │  //Win*   │ Del/Ctrl  │
└───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘     └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
                                                 ┌───────────┬───────────┐     ┌───────────┬───────────┐
                                                 │ Enter/L1  │ Esc/Ctrl* │     │Space/Ctrl*│  Bksp/L2  │
                                                 └───────────┴───────────┘     └───────────┴───────────┘
```

## Modifiers

| Key   | Position | Finger            | Tap       | Hold on Windows | Hold on Mac |
| ----- | -------- | ----------------- | --------- | --------------- | ----------- |
| Esc   | `LT2`    | left inner thumb  | Esc       | Ctrl            | Cmd         |
| Space | `RT1`    | right inner thumb | Space     | Ctrl            | Cmd         |
| Z     | `L4-2`   | left pinky        | Z         | Win             | Cmd         |
| /     | `R4-5`   | right pinky       | /         | Win             | Cmd         |
| Ctrl  | `L4-1`   | left outer pinky  | none      | Ctrl            | Ctrl        |
| Del   | `R4-6`   | right outer pinky | Delete    | Ctrl            | Ctrl        |
| X     | `L4-3`   | left ring         | X         | Alt             | Option      |
| `.`   | `R4-4`   | right ring        | .         | Alt             | Option      |
| Shift | `L3-1`   | left outer pinky  | none      | Shift           | Shift       |
| `'`   | `R3-6`   | right outer pinky | '         | Shift           | Shift       |
| Enter | `LT1`    | left outer thumb  | Enter     | layer 1         | layer 1     |
| Bksp  | `RT2`    | right outer thumb | Backspace | layer 2         | layer 2     |

Each hand has its own set: an app command on the inner thumb, a Ctrl that never
changes on the outer pinky, a Windows key on the pinky, and an Alt on the ring
finger. That is what keeps every shortcut on one hand.

## Layer 1 — hold Enter (`LT1`)

Symbols on the left, character-by-character movement on the right, F keys on the
top row.

```
┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐     ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
│     _     │    F1     │    F2     │    F3     │    F4     │    F5     │     │    F6     │    F7     │    F8     │    F9     │    F10    │    F11    │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│     _     │ Mac mode  │     \     │     |     │     {     │     }     │     │ Wheel up  │   Home    │    Up     │    End    │   PgUp    │    F12    │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│ Del word  │     _     │     <     │     >     │     [     │     ]     │     │Wheel down │   Left    │   Down    │   Right   │   PgDn    │     _     │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│     _     │     _     │     _     │     _     │Paste plain│ \ + Enter │     │     _     │     _     │     _     │     _     │     _     │     _     │
└───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘     └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
                                                 ┌───────────┬───────────┐     ┌───────────┬───────────┐
                                                 │     _     │     _     │     │     _     │     _     │
                                                 └───────────┴───────────┘     └───────────┴───────────┘
```

## Layer 2 — hold Bksp (`RT2`)

Lighting on the left, word-by-word movement on the right, at the same positions
layer 1 uses for single characters.

```
┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐     ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
│     _     │    F1     │    F2     │    F3     │    F4     │    F5     │     │    F6     │    F7     │    F8     │    F9     │    F10    │    F11    │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│     _     │     _     │     _     │     _     │ Bright -  │ Bright +  │     │ Wheel up  │Line start │    Up     │ Line end  │   PgUp    │    F12    │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│     _     │     _     │     _     │     _     │RGB on/off │ RGB next  │     │Wheel down │ Word left │   Down    │Word right │   PgDn    │     _     │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│     _     │     _     │     _     │     _     │ RGB solid │Layer LEDs │     │Paste plain│ \ + Enter │ Prev tab  │ Next tab  │     _     │     _     │
└───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘     └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
                                                 ┌───────────┬───────────┐     ┌───────────┬───────────┐
                                                 │     _     │     _     │     │     _     │     _     │
                                                 └───────────┴───────────┘     └───────────┴───────────┘
```

## Colours

Each key glows in one of seven colours. Blue is the base layer's colour and
appears nowhere else: a blue key tells you no layer is held, so F1 can never be
mistaken for `1`. On layers 1 and 2 only the keys the layer changes light up,
plus the held thumb; everything marked `_` is dark.

A colour keeps its meaning for a group that exists on both layers (arrows,
PgUp/PgDn, wheel, Home/End and Line start/end, Paste plain and `\` + Enter, F
keys). Beyond that a colour is reused, and may mean one thing on one layer and
another elsewhere. Two groups that touch never share a colour. The grid for the
layer you are on is the reference.

| Colour | Looks like | Base                                                                                   | Layer 1                                                                                                              | Layer 2                                                                           |
| ------ | ---------- | -------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------- |
| blue   | `#0000FF`  | keys that type: letters, digits, `` ` ``, Tab, `-`, `=`, `;`, `,`                      | —                                                                                                                    | —                                                                                 |
| cyan   | `#00FFF6`  | —                                                                                      | arrows (`R2-3`, `R3-2`, `R3-3`, `R3-4`); `<` `>` (`L3-3`, `L3-4`)                                                    | arrows; RGB on/off, RGB next, RGB solid (`L3-5`, `L3-6`, `L4-5`)                  |
| green  | `#42FF00`  | layer thumbs Enter (`LT1`) and Bksp (`RT2`)                                            | F1–F12; the held Enter; `[` `]` (`L3-5`, `L3-6`)                                                                     | F1–F12; the held Bksp; Word left, Word right (`R3-2`, `R3-4`)                     |
| orange | `#FF7E00`  | 5/Click (`L1-6`), the hold is a mouse click                                            | Wheel up, Wheel down (`R2-1`, `R3-1`)                                                                                | Wheel up, Wheel down; Prev tab, Next tab (`R4-3`, `R4-4`)                         |
| purple | `#B400FF`  | fixed modifiers: Shift (`L3-1`, `R3-6`), Ctrl (`L4-1`, `R4-6`), Alt (`L4-3`, `R4-4`)   | Home, End (`R2-2`, `R2-4`); `\` `\|` (`L2-3`, `L2-4`); Del word (`L3-1`), Paste plain (`L4-5`), `\` + Enter (`L4-6`) | Line start, Line end (`R2-2`, `R2-4`); Paste plain (`R4-1`), `\` + Enter (`R4-2`) |
| red    | `#FF001E`  | keys that change with the system: Esc (`LT2`), Space (`RT1`), Z (`L4-2`), `/` (`R4-5`) | PgUp, PgDn (`R2-5`, `R3-5`); `{` `}` (`L2-5`, `L2-6`)                                                                | PgUp, PgDn; Bright −, Bright + (`L2-5`, `L2-6`)                                   |
| white  | `#FFFFFF`  | Mac mode indicator on Ctrl (`L4-1`)                                                    | Mac mode (`L2-2`); the indicator                                                                                     | Layer LEDs (`L4-6`); the indicator                                                |

White on `L4-1` means Mac mode, on every layer. The two white keys on the layers
are the switches that change how the board itself behaves, the same thing the
indicator reports. Orange is the one colour that is not neon; it marks the mouse
keys and the two tab keys and nothing else.

### Base

```
┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐     ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
│   blue    │   blue    │   blue    │   blue    │   blue    │  orange   │     │   blue    │   blue    │   blue    │   blue    │   blue    │   blue    │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│   blue    │   blue    │   blue    │   blue    │   blue    │   blue    │     │   blue    │   blue    │   blue    │   blue    │   blue    │   blue    │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│  purple   │   blue    │   blue    │   blue    │   blue    │   blue    │     │   blue    │   blue    │   blue    │   blue    │   blue    │  purple   │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│  purple   │    red    │  purple   │   blue    │   blue    │   blue    │     │   blue    │   blue    │   blue    │  purple   │    red    │  purple   │
└───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘     └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
                                                 ┌───────────┬───────────┐     ┌───────────┬───────────┐
                                                 │   green   │    red    │     │    red    │   green   │
                                                 └───────────┴───────────┘     └───────────┴───────────┘
```

### Layer 1 — hold Enter (`LT1`)

```
┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐     ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
│           │   green   │   green   │   green   │   green   │   green   │     │   green   │   green   │   green   │   green   │   green   │   green   │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│           │   white   │  purple   │  purple   │    red    │    red    │     │  orange   │  purple   │   cyan    │  purple   │    red    │   green   │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│  purple   │           │   cyan    │   cyan    │   green   │   green   │     │  orange   │   cyan    │   cyan    │   cyan    │    red    │           │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│           │           │           │           │  purple   │  purple   │     │           │           │           │           │           │           │
└───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘     └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
                                                 ┌───────────┬───────────┐     ┌───────────┬───────────┐
                                                 │   green   │           │     │           │           │
                                                 └───────────┴───────────┘     └───────────┴───────────┘
```

### Layer 2 — hold Bksp (`RT2`)

```
┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐     ┌───────────┬───────────┬───────────┬───────────┬───────────┬───────────┐
│           │   green   │   green   │   green   │   green   │   green   │     │   green   │   green   │   green   │   green   │   green   │   green   │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│           │           │           │           │    red    │    red    │     │  orange   │  purple   │   cyan    │  purple   │    red    │   green   │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│           │           │           │           │   cyan    │   cyan    │     │  orange   │   green   │   cyan    │   green   │    red    │           │
├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤     ├───────────┼───────────┼───────────┼───────────┼───────────┼───────────┤
│           │           │           │           │   cyan    │   white   │     │  purple   │  purple   │  orange   │  orange   │           │           │
└───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘     └───────────┴───────────┴───────────┴───────────┴───────────┴───────────┘
                                                 ┌───────────┬───────────┐     ┌───────────┬───────────┐
                                                 │           │           │     │           │   green   │
                                                 └───────────┴───────────┘     └───────────┴───────────┘
```

## App shortcuts, left hand

Hold Esc (`LT2`), the left inner thumb. Sends Ctrl on Windows, Cmd on a Mac.

| Purpose         | Combo    | Position       | Fingers                   |
| --------------- | -------- | -------------- | ------------------------- |
| copy            | Esc + C  | `LT2` + `L4-4` | inner thumb + middle      |
| paste           | Esc + V  | `LT2` + `L4-5` | inner thumb + index       |
| cut             | Esc + X  | `LT2` + `L4-3` | inner thumb + ring        |
| undo            | Esc + Z  | `LT2` + `L4-2` | inner thumb + pinky       |
| select all      | Esc + A  | `LT2` + `L3-2` | inner thumb + pinky       |
| save            | Esc + S  | `LT2` + `L3-3` | inner thumb + ring        |
| find            | Esc + F  | `LT2` + `L3-5` | inner thumb + index       |
| refresh         | Esc + R  | `LT2` + `L2-5` | inner thumb + index       |
| close tab       | Esc + W  | `LT2` + `L2-3` | inner thumb + ring        |
| new tab         | Esc + T  | `LT2` + `L2-6` | inner thumb + inner index |
| quit            | Esc + Q  | `LT2` + `L2-2` | inner thumb + pinky       |
| editor terminal | Esc + \` | `LT2` + `L1-1` | inner thumb + outer pinky |

## App shortcuts, right hand

Hold Space (`RT1`), the right inner thumb. Same modifier, other hand.

| Purpose     | Combo     | Position       | Fingers                   |
| ----------- | --------- | -------------- | ------------------------- |
| address bar | Space + L | `RT1` + `R3-4` | inner thumb + ring        |
| new window  | Space + N | `RT1` + `R4-1` | inner thumb + inner index |
| minimise    | Space + M | `RT1` + `R4-2` | inner thumb + index       |
| insert link | Space + K | `RT1` + `R3-3` | inner thumb + middle      |
| open        | Space + O | `RT1` + `R2-4` | inner thumb + ring        |
| print       | Space + P | `RT1` + `R2-5` | inner thumb + pinky       |
| zoom out    | Space + - | `RT1` + `R1-6` | inner thumb + outer pinky |
| zoom in     | Space + = | `RT1` + `R2-6` | inner thumb + outer pinky |
| zoom reset  | Space + 0 | `RT1` + `R1-5` | inner thumb + pinky       |

## Terminal

These send the same bytes on both systems, so nothing changes between the two.
Left hand holds Ctrl (`L4-1`), right hand holds Del (`R4-6`).

Combo is which keys to press, Sends is what the computer receives. On the left
hand the two read alike, because the key being held is the one printed Ctrl.

| Purpose            | Combo    | Position        | Fingers                   | Sends  |
| ------------------ | -------- | --------------- | ------------------------- | ------ |
| cancel             | Ctrl + C | `L4-1` + `L4-4` | outer pinky + middle      | Ctrl+C |
| line start         | Ctrl + A | `L4-1` + `L3-2` | outer pinky + pinky       | Ctrl+A |
| line end           | Ctrl + E | `L4-1` + `L2-4` | outer pinky + middle      | Ctrl+E |
| end of input       | Ctrl + D | `L4-1` + `L3-4` | outer pinky + middle      | Ctrl+D |
| search history     | Ctrl + R | `L4-1` + `L2-5` | outer pinky + index       | Ctrl+R |
| delete word back   | Ctrl + W | `L4-1` + `L2-3` | outer pinky + ring        | Ctrl+W |
| suspend            | Ctrl + Z | `L4-1` + `L4-2` | outer pinky + pinky       | Ctrl+Z |
| clear line         | Del + U  | `R4-6` + `R2-2` | outer pinky + index       | Ctrl+U |
| clear screen       | Del + L  | `R4-6` + `R3-4` | outer pinky + ring        | Ctrl+L |
| backspace          | Del + H  | `R4-6` + `R3-1` | outer pinky + inner index | Ctrl+H |
| next history       | Del + N  | `R4-6` + `R4-1` | outer pinky + inner index | Ctrl+N |
| previous history   | Del + P  | `R4-6` + `R2-5` | outer pinky + pinky       | Ctrl+P |
| cut to end of line | Del + K  | `R4-6` + `R3-3` | outer pinky + middle      | Ctrl+K |

Copy and cancel are separate keys on a Mac. Esc + C (`LT2` + `L4-4`) copies,
Ctrl + C (`L4-1` + `L4-4`) cancels.

## Movement

Layer 1 moves by single characters, layer 2 by words. Same positions in both, so
only the thumb changes.

| Purpose               | Combo          | Position                     | Fingers                           | Windows          | Mac                |
| --------------------- | -------------- | ---------------------------- | --------------------------------- | ---------------- | ------------------ |
| left, down, right     | Enter + J K L  | `LT1` + `R3-2` `R3-3` `R3-4` | outer thumb + index, middle, ring | arrows           | arrows             |
| up                    | Enter + I      | `LT1` + `R2-3`               | outer thumb + middle              | arrow            | arrow              |
| page up, page down    | Enter + P `;`  | `LT1` + `R2-5` `R3-5`        | outer thumb + pinky               | page keys        | page keys          |
| word left, word right | Bksp + J L     | `RT2` + `R3-2` `R3-4`        | outer thumb + index, ring         | Ctrl+arrow       | Option+arrow       |
| line start, line end  | Bksp + U O     | `RT2` + `R2-2` `R2-4`        | outer thumb + index, ring         | Home, End        | Cmd+arrow          |
| delete word back      | Enter + Shift  | `LT1` + `L3-1`               | outer thumb + outer pinky         | Ctrl+Bksp        | Option+Bksp        |
| select by word        | Bksp + `'` + J | `RT2` + `R3-6` + `R3-2`      | outer thumb + outer pinky + index | Shift+Ctrl+arrow | Shift+Option+arrow |

## Switching

| Purpose             | Combo                | Position                  | Fingers                   | Windows         | Mac             |
| ------------------- | -------------------- | ------------------------- | ------------------------- | --------------- | --------------- |
| switch app, cycling | Esc + Tab repeatedly | `LT2` + `L2-1` repeatedly | inner thumb + outer pinky | Alt+Tab Tab Tab | Cmd+Tab Tab Tab |
| next tab            | Bksp + `.`           | `RT2` + `R4-4`            | outer thumb + ring        | Ctrl+Tab        | Ctrl+Tab        |
| previous tab        | Bksp + `,`           | `RT2` + `R4-3`            | outer thumb + middle      | Ctrl+Shift+Tab  | Ctrl+Shift+Tab  |

Hold the thumb and tap the pinky as many times as you want. The modifier stays
down, so the window list stays open through a pause and arrow keys work inside
it. Releasing the thumb picks the highlighted window.

## Windows key

| Purpose       | Combo                       | Position                         | Fingers            |
| ------------- | --------------------------- | -------------------------------- | ------------------ |
| Start menu    | hold Z past 200 ms, release | hold `L4-2` past 200 ms, release | left pinky         |
| show desktop  | Z + D                       | `L4-2` + `L3-4`                  | pinky + middle     |
| file explorer | Z + E                       | `L4-2` + `L2-4`                  | pinky + middle     |
| run           | Z + R                       | `L4-2` + `L2-5`                  | pinky + index      |
| lock screen   | / + L                       | `R4-5` + `R3-4`                  | right pinky + ring |

## Mac system shortcuts

| Purpose                  | Combo           | Position                | Fingers                         | Sends        |
| ------------------------ | --------------- | ----------------------- | ------------------------------- | ------------ |
| switch window in app     | Esc + \`        | `LT2` + `L1-1`          | inner thumb + outer pinky       | Cmd+backtick |
| hide app                 | Space + H       | `RT1` + `R3-1`          | inner thumb + inner index       | Cmd+H        |
| screenshot an area       | Esc + Shift + 4 | `LT2` + `L3-1` + `L1-5` | inner thumb, outer pinky, index | Cmd+Shift+4  |
| paste without formatting | Enter + V       | `LT1` + `L4-5`          | outer thumb + index             | Cmd+Shift+V  |

## Mac mode

The keyboard decides at plug-in. While Mac mode is on, the key printed Ctrl
(`L4-1`) glows white.

To change it by hand: Enter + Q (`LT1` + `L2-2`), left outer thumb plus left
pinky. Press again to change back. The choice is not remembered; unplugging and
plugging in decides again.

## What needs both hands

Five combos. Each is here because two keys it needs sit on the same finger, or
on the same hand as the layer key.

- **Spotlight on a Mac**, Cmd+Space. Space is the right inner thumb and Cmd is
  the left inner thumb.
- **Force Quit on a Mac**, Cmd+Option+Escape. Escape is the tap on the same
  thumb that holds Cmd, so Cmd has to come from the right thumb.
- **Backward app switching**, Alt+Shift+Tab or Cmd+Shift+Tab. Tab and the left
  Shift are both the left outer pinky, so Shift comes from the key printed `'`
  (`R3-6`).
- **Window snapping on Windows**, Windows key plus an arrow. The arrows live on
  a layer held by a thumb on the other hand from the Windows key.
- **Mission Control and desktop switching on a Mac**, Ctrl plus an arrow. Same
  reason as snapping.
