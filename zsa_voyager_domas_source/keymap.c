#include QMK_KEYBOARD_H
#include "version.h"
#include <lib/lib8tion/lib8tion.h>
#define MOON_LED_LEVEL LED_LEVEL
#ifndef ZSA_SAFE_RANGE
#define ZSA_SAFE_RANGE SAFE_RANGE
#endif

enum custom_keycodes {
    EFFECTS_TOGGLE = ZSA_SAFE_RANGE,
    BRIGHT_CYCLE,
    BACKSLASH_ENTER,
    MAC_TOGGLE,
    SWITCH_TAB,
    PASTE_PLAIN,
    WORD_LEFT,
    WORD_RIGHT,
    LINE_START,
    LINE_END,
    DELETE_WORD,
    DELETE_LINE,
};

#define APP_CMD_L  LT(0, KC_ESCAPE)
#define APP_CMD_R  LT(0, KC_SPACE)
#define NUM5_CLICK LT(0, KC_5)
#define NEXT_TAB   LCTL(KC_TAB)
#define PREV_TAB   LCTL(LSFT(KC_TAB))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_voyager(
        KC_GRAVE,      KC_1,          KC_2,          KC_3,          KC_4,          NUM5_CLICK,              KC_6,          KC_7,          KC_8,          KC_9,          KC_0,             KC_MINUS,
        SWITCH_TAB,    KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,                    KC_Y,          KC_U,          KC_I,          KC_O,          KC_P,             KC_EQUAL,
        KC_LEFT_SHIFT, KC_A,          KC_S,          KC_D,          KC_F,          KC_G,                    KC_H,          KC_J,          KC_K,          KC_L,          KC_SCLN,          RSFT_T(KC_QUOTE),
        KC_LEFT_CTRL,  LGUI_T(KC_Z),  LALT_T(KC_X),  KC_C,          KC_V,          KC_B,                    KC_N,          KC_M,          KC_COMMA,      RALT_T(KC_DOT), RGUI_T(KC_SLASH), RCTL_T(KC_DELETE),
                                                                    LT(1, KC_ENTER), APP_CMD_L,             APP_CMD_R,     LT(2, KC_BSPC)
    ),
    [1] = LAYOUT_voyager(
        PREV_TAB,      KC_F1,         KC_F2,         KC_F3,         KC_F4,         KC_F5,                   KC_F6,         KC_F7,         KC_F8,         KC_F9,         KC_F10,           KC_F11,
        NEXT_TAB,      _______,       KC_BSLS,       KC_PIPE,       KC_LCBR,       KC_RCBR,                 _______,       _______,       _______,       _______,       _______,          KC_F12,
        _______,       _______,       KC_LABK,       KC_RABK,       KC_LBRC,       KC_RBRC,                 _______,       _______,       _______,       _______,       _______,          _______,
        _______,       _______,       _______,       _______,       PASTE_PLAIN,   BACKSLASH_ENTER,         _______,       _______,       _______,       _______,       _______,          MAC_TOGGLE,
                                                                    _______,       _______,                 _______,       _______
    ),
    [2] = LAYOUT_voyager(
        _______,       KC_F1,         KC_F2,         KC_F3,         KC_F4,         KC_F5,                   KC_F6,         KC_F7,         KC_F8,         KC_F9,         KC_F10,           KC_F11,
        _______,       _______,       _______,       _______,       BRIGHT_CYCLE,  _______,                 KC_MS_WH_UP,   LINE_START,    KC_UP,         LINE_END,      KC_PAGE_UP,       KC_F12,
        _______,       _______,       _______,       _______,       RGB_TOG,       EFFECTS_TOGGLE,           KC_MS_WH_DOWN, KC_LEFT,       KC_DOWN,       KC_RIGHT,      KC_PGDN,          _______,
        _______,       _______,       _______,       _______,       _______,       _______,                 _______,       WORD_LEFT,     _______,       WORD_RIGHT,    DELETE_LINE,      DELETE_WORD,
                                                                    _______,       _______,                 _______,       _______
    ),
};

const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_voyager(
    'L', 'L', 'L', 'L', 'L', '*',      'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R', 'R',
    'L', '*', '*', 'L', 'L', 'L',      'R', 'R', 'R', '*', '*', '*',
                        '*', '*',      '*', '*'
);

static bool mac_mode;
static bool effects_on = true;
static bool app_switch_alt_held;

// A thumb that taps one key and holds a modifier that depends on the mode.
typedef struct {
    uint16_t tap_keycode;
    uint8_t  windows_mod;
    uint8_t  mac_mod;
    uint8_t  held;   // the modifier now down, 0 if none
    bool     alone;  // nothing else has been pressed since it went down
} app_command_t;

static app_command_t left_app_command  = { .tap_keycode = KC_ESCAPE, .windows_mod = KC_LEFT_CTRL, .mac_mod = KC_LEFT_GUI };
static app_command_t right_app_command = { .tap_keycode = KC_SPACE, .windows_mod = KC_RIGHT_CTRL, .mac_mod = KC_RIGHT_GUI };

static void release_app_command(app_command_t *command) {
    if (command->held) {
        unregister_code(command->held);
        command->held = 0;
    }
}

static void end_app_switch(void) {
    if (app_switch_alt_held) {
        unregister_code(KC_LEFT_ALT);
        app_switch_alt_held = false;
    }
}

static void set_mac_mode(bool on) {
    if (mac_mode == on) {
        return;
    }
    end_app_switch();
    release_app_command(&left_app_command);
    release_app_command(&right_app_command);
    mac_mode = on;
}

bool process_detected_host_os_user(os_variant_t os) {
    switch (os) {
        case OS_MACOS:
        case OS_IOS:
            set_mac_mode(true);
            break;
        case OS_WINDOWS:
        case OS_LINUX:
            set_mac_mode(false);
            break;
        default:
            break;
    }
    return true;
}

typedef struct {
    uint16_t keycode;
    uint16_t tapping_term;
    bool     hold_on_other_key_press;
    bool     retro_tapping;
} tap_hold_t;

static const tap_hold_t tap_holds[] = {
    { LGUI_T(KC_Z),      200, false, false },
    { RGUI_T(KC_SLASH),  200, false, false },
    { LALT_T(KC_X),      200, false, false },
    { RALT_T(KC_DOT),    200, false, false },
    { RCTL_T(KC_DELETE), 200, false, true  },
    { NUM5_CLICK,        200, false, false },
    { RSFT_T(KC_QUOTE),  150, true,  true  },
};

static const tap_hold_t tap_hold_default = { 0, TAPPING_TERM, false, true };

static const tap_hold_t *tap_hold_for(uint16_t keycode) {
    for (uint8_t i = 0; i < ARRAY_SIZE(tap_holds); i++) {
        if (tap_holds[i].keycode == keycode) {
            return &tap_holds[i];
        }
    }
    return &tap_hold_default;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return tap_hold_for(keycode)->tapping_term;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    return tap_hold_for(keycode)->hold_on_other_key_press;
}

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    return tap_hold_for(keycode)->retro_tapping;
}

bool is_flow_tap_key(uint16_t keycode) {
    if ((get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) != 0) {
        return false;
    }
    switch (get_tap_keycode(keycode)) {
        case KC_SPACE:
        case KC_A ... KC_Z:
        case KC_1 ... KC_0:
        case KC_DOT:
        case KC_COMMA:
        case KC_SCLN:
        case KC_SLASH:
            return true;
    }
    return false;
}

typedef struct {
    uint16_t keycode;
    uint16_t windows;
    uint16_t mac;
} os_keycode_t;

static const os_keycode_t os_keycodes[] = {
    { WORD_LEFT,   LCTL(KC_LEFT),    LALT(KC_LEFT)    },
    { WORD_RIGHT,  LCTL(KC_RIGHT),   LALT(KC_RIGHT)   },
    { LINE_START,  KC_HOME,          LGUI(KC_LEFT)    },
    { LINE_END,    KC_END,           LGUI(KC_RIGHT)   },
    { DELETE_WORD, LCTL(KC_BSPC),    LALT(KC_BSPC)    },
    { PASTE_PLAIN, LCTL(LSFT(KC_V)), LGUI(LSFT(KC_V)) },
};

static uint16_t os_keycode_sent[ARRAY_SIZE(os_keycodes)];

static int8_t os_keycode_index(uint16_t keycode) {
    for (uint8_t i = 0; i < ARRAY_SIZE(os_keycodes); i++) {
        if (os_keycodes[i].keycode == keycode) {
            return (int8_t)i;
        }
    }
    return -1;
}

static uint16_t os_keycode_for(int8_t index) {
    return mac_mode ? os_keycodes[index].mac : os_keycodes[index].windows;
}

static uint16_t os_shortcut(uint16_t keycode) {
    return os_keycode_for(os_keycode_index(keycode));
}

static bool process_os_keycode(int8_t index, keyrecord_t *record) {
    if (record->event.pressed) {
        os_keycode_sent[index] = os_keycode_for(index);
        register_code16(os_keycode_sent[index]);
    } else if (os_keycode_sent[index]) {
        unregister_code16(os_keycode_sent[index]);
        os_keycode_sent[index] = 0;
    }
    return false;
}

static bool process_app_command(app_command_t *command, keyrecord_t *record) {
    if (record->tap.count) {
        if (record->event.pressed) {
            register_code16(command->tap_keycode);
        } else {
            unregister_code16(command->tap_keycode);
        }
        return false;
    }
    if (record->event.pressed) {
        command->alone = true;
        command->held  = mac_mode ? command->mac_mod : command->windows_mod;
        register_code(command->held);
    } else {
        release_app_command(command);
        // A hold that nothing interrupted was a slow tap. QMK's own retro tapping
        // cannot reach this key: it sits at the end of process_action, and
        // returning false from here means process_action never runs.
        if (command->alone) {
            tap_code16(command->tap_keycode);
        }
    }
    return false;
}

// One key cycles brightness through quarters of the board's maximum:
// 0, 25, 50, 75, 100 %, then back to 0. The current value is snapped to the
// nearest stage first, so a value left by an older build or by Oryx lands on
// the ladder after one press.
static const uint8_t brightness_stages[] = {
    0,
    RGB_MATRIX_MAXIMUM_BRIGHTNESS * 1 / 4,
    RGB_MATRIX_MAXIMUM_BRIGHTNESS * 2 / 4,
    RGB_MATRIX_MAXIMUM_BRIGHTNESS * 3 / 4,
    RGB_MATRIX_MAXIMUM_BRIGHTNESS,
};

static void cycle_brightness(void) {
    uint8_t value   = rgb_matrix_get_val();
    uint8_t nearest = 0;
    for (uint8_t i = 1; i < ARRAY_SIZE(brightness_stages); i++) {
        if (abs((int16_t)brightness_stages[i] - value) < abs((int16_t)brightness_stages[nearest] - value)) {
            nearest = i;
        }
    }
    uint8_t next = (nearest + 1) % ARRAY_SIZE(brightness_stages);
    rgb_matrix_sethsv(rgb_matrix_get_hue(), rgb_matrix_get_sat(), brightness_stages[next]);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode != APP_CMD_L) {
        left_app_command.alone = false;
    }
    if (keycode != APP_CMD_R) {
        right_app_command.alone = false;
    }

    int8_t os_index = os_keycode_index(keycode);
    if (os_index >= 0) {
        return process_os_keycode(os_index, record);
    }

    switch (keycode) {
        case QK_MODS ... QK_MODS_MAX:
            // Mouse keys with modifiers work inconsistently across operating systems; this
            // makes sure the modifiers are applied to the mouse key that was pressed. No key
            // in the keymap is one today, so this is a guard for whoever adds one.
            if (IS_MOUSE_KEYCODE(QK_MODS_GET_BASIC_KEYCODE(keycode))) {
                if (record->event.pressed) {
                    add_mods(QK_MODS_GET_MODS(keycode));
                    send_keyboard_report();
                    wait_ms(2);
                    register_code(QK_MODS_GET_BASIC_KEYCODE(keycode));
                    return false;
                } else {
                    wait_ms(2);
                    del_mods(QK_MODS_GET_MODS(keycode));
                }
            }
            break;

        case APP_CMD_L:
            if (!record->event.pressed) {
                end_app_switch();
            }
            return process_app_command(&left_app_command, record);

        case APP_CMD_R:
            return process_app_command(&right_app_command, record);

        case SWITCH_TAB:
            if (record->event.pressed) {
                // Windows switches apps on Alt+Tab, so the thumb's Ctrl is traded for Alt and kept
                // down until the thumb lifts. That is what lets repeated taps cycle the window list.
                if (!mac_mode && left_app_command.held && !app_switch_alt_held) {
                    release_app_command(&left_app_command);
                    register_code(KC_LEFT_ALT);
                    app_switch_alt_held = true;
                }
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            return false;

        case MAC_TOGGLE:
            if (record->event.pressed) {
                set_mac_mode(!mac_mode);
            }
            return false;

        case BACKSLASH_ENTER:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_BSLS) SS_DELAY(50) SS_TAP(X_ENTER));
            }
            return false;

        case DELETE_LINE:
            if (record->event.pressed) {
                // Windows has no delete-to-line-start keystroke, so both systems select
                // to the line start and delete the selection.
                tap_code16(LSFT(os_shortcut(LINE_START)));
                tap_code(KC_BSPC);
            }
            return false;

        case NUM5_CLICK:
            if (record->tap.count > 0) {
                if (record->event.pressed) {
                    register_code16(KC_5);
                } else {
                    unregister_code16(KC_5);
                }
            } else {
                if (record->event.pressed) {
                    register_code16(KC_MS_BTN1);
                } else {
                    unregister_code16(KC_MS_BTN1);
                }
            }
            return false;

        case EFFECTS_TOGGLE:
            if (record->event.pressed) {
                effects_on = !effects_on;
            }
            return false;

        case BRIGHT_CYCLE:
            if (record->event.pressed) {
                cycle_brightness();
            }
            return false;
    }
    return true;
}

// Scales after the conversion, not before. hsv_to_rgb runs the value through
// the CIE 1931 curve, so scaling the value first would dim the board at full
// and change every stage of the brightness ladder.
static RGB rgb_at_brightness(HSV hsv) {
    RGB   rgb = hsv_to_rgb(hsv);
    float f   = (float)rgb_matrix_get_val() / UINT8_MAX;
    return (RGB){ f * rgb.r, f * rgb.g, f * rgb.b };
}

void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

enum glow {
    GLOW_OFF,
    GLOW_RED,
    GLOW_ORANGE,
    GLOW_LIME,
    GLOW_GREEN,
    GLOW_CYAN,
    GLOW_BLUE,
    GLOW_PURPLE,
    GLOW_PINK,
    GLOW_WHITE,
};

static const HSV glow_palette[] = {
    [GLOW_OFF]    = {   0,   0,   0 },
    [GLOW_RED]    = { 254, 255, 255 },
    [GLOW_ORANGE] = {  28, 255, 255 },
    [GLOW_LIME]   = {  60, 255, 255 },
    [GLOW_GREEN]  = {  92, 255, 255 },
    [GLOW_CYAN]   = { 128, 255, 255 },
    [GLOW_BLUE]   = { 164, 255, 255 },
    [GLOW_PURPLE] = { 194, 255, 255 },
    [GLOW_PINK]   = { 224, 255, 255 },
    [GLOW_WHITE]  = {   0,   0, 255 },
};

// Named by colour, not by key group: one colour serves different groups on
// different layers. What each colour means where is the Colours section of
// LAYOUT.md; the rule for picking them is the Lighting section of FIRMWARE.md.
// GLOW_OFF must stay 0, because LAYOUT_voyager fills the unused matrix cells
// with KC_NO. Keys whose hold changes with the system, and the Mac mode key,
// are entered as their Windows-mode colour; while Mac mode is on,
// rgb_matrix_indicators_user paints them white instead.
const uint8_t PROGMEM glowmap[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_voyager(
        GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_RED,         GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,
        GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,        GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,
        GLOW_ORANGE,  GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,        GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_ORANGE,
        GLOW_ORANGE,  GLOW_PINK,    GLOW_ORANGE,  GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,        GLOW_BLUE,    GLOW_BLUE,    GLOW_BLUE,    GLOW_ORANGE,  GLOW_PINK,    GLOW_ORANGE,
                                                                GLOW_GREEN,   GLOW_PINK,        GLOW_PINK,    GLOW_GREEN
    ),
    [1] = LAYOUT_voyager(
        GLOW_PINK,    GLOW_LIME,    GLOW_LIME,    GLOW_LIME,    GLOW_LIME,    GLOW_LIME,        GLOW_LIME,    GLOW_LIME,    GLOW_LIME,    GLOW_LIME,    GLOW_LIME,    GLOW_LIME,
        GLOW_PINK,    GLOW_OFF,     GLOW_RED,     GLOW_RED,     GLOW_PURPLE,  GLOW_PURPLE,      GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_LIME,
        GLOW_OFF,     GLOW_OFF,     GLOW_CYAN,    GLOW_CYAN,    GLOW_ORANGE,  GLOW_ORANGE,      GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,
        GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_PINK,    GLOW_PINK,        GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_PINK,
                                                                GLOW_GREEN,   GLOW_OFF,         GLOW_OFF,     GLOW_OFF
    ),
    [2] = LAYOUT_voyager(
        GLOW_OFF,     GLOW_LIME,    GLOW_LIME,    GLOW_LIME,    GLOW_LIME,    GLOW_LIME,        GLOW_LIME,    GLOW_LIME,    GLOW_LIME,    GLOW_LIME,    GLOW_LIME,    GLOW_LIME,
        GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_WHITE,   GLOW_OFF,         GLOW_RED,     GLOW_CYAN,    GLOW_PURPLE,  GLOW_CYAN,    GLOW_PINK,    GLOW_LIME,
        GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_WHITE,   GLOW_WHITE,       GLOW_RED,     GLOW_PURPLE,  GLOW_PURPLE,  GLOW_PURPLE,  GLOW_PINK,    GLOW_OFF,
        GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,         GLOW_OFF,     GLOW_ORANGE,  GLOW_OFF,     GLOW_ORANGE,  GLOW_CYAN,    GLOW_ORANGE,
                                                                GLOW_OFF,     GLOW_OFF,         GLOW_OFF,     GLOW_GREEN
    ),
};

// Linger: a pressed key dims by a quarter, stays there, and comes back to full at
// the end of linger_ms, on an easeInExpo curve: f(p) = 2^(10(p-1)), f(0) = 0.
// The table holds f at 17 points times 255; a float exponent per LED per frame
// is not worth it. The dip is a fraction of the key's own value, so it scales
// with the global brightness instead of fighting it. Dark keys stay dark.
static const uint16_t linger_ms = 400;

static const uint8_t ease_in_expo[17] = { 0, 0, 1, 1, 1, 2, 3, 5, 8, 12, 19, 29, 45, 70, 107, 165, 255 };

static uint8_t eased(uint16_t age) {
    uint32_t scaled = (uint32_t)age * 16 * 255 / linger_ms;
    uint8_t  index  = scaled / 255;
    uint8_t  frac   = scaled % 255;
    if (index >= 16) {
        return 255;
    }
    return lerp8by8(ease_in_expo[index], ease_in_expo[index + 1], frac);
}

static HSV linger(uint8_t led, HSV hsv) {
    if (!effects_on) {
        return hsv;
    }
    uint16_t age = linger_ms;
    for (uint8_t i = 0; i < g_last_hit_tracker.count; i++) {
        if (g_last_hit_tracker.index[i] == led && g_last_hit_tracker.tick[i] < age) {
            age = g_last_hit_tracker.tick[i];
        }
    }
    if (age >= linger_ms) {
        return hsv;
    }
    uint8_t dip = hsv.v / 4;
    hsv.v       = hsv.v - dip + (uint16_t)dip * eased(age) / 255;
    return hsv;
}

// Pulse: the mode keys breathe between full and 60 % over about 3 s, all in
// step. The dip is squared, so the light rests at full for most of the cycle
// and sinks only briefly. g_rgb_timer / 12 makes one sin8 cycle 3072 ms.
static HSV pulse(HSV hsv) {
    if (!effects_on) {
        return hsv;
    }
    uint8_t dip = 255 - sin8(g_rgb_timer / 12);
    dip         = (uint16_t)dip * dip / 255;
    hsv.v -= (hsv.v * 4 / 10) * dip / 255;
    return hsv;
}

static void set_led_hsv(uint8_t led, HSV hsv) {
    RGB rgb = rgb_at_brightness(linger(led, hsv));
    rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
}

// The keys that send something different in Mac mode, plus the key that flips it.
// They show the mode: their glowmap colour on Windows, white on a Mac. They
// pulse in both modes, so they stand out from every other key without owning
// a colour of their own.
static const uint16_t mode_keys[] = { APP_CMD_L, APP_CMD_R, LGUI_T(KC_Z), RGUI_T(KC_SLASH), MAC_TOGGLE };

static bool is_mode_key(uint16_t keycode) {
    for (uint8_t i = 0; i < ARRAY_SIZE(mode_keys); i++) {
        if (mode_keys[i] == keycode) {
            return true;
        }
    }
    return false;
}

// Mode keys are found by keycode, not by position, so moving one in the keymap
// moves its light with it. A transparent cell is never a mode key and keeps its
// glowmap colour.
static void paint_layer(uint8_t layer) {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t led = g_led_config.matrix_co[row][col];
            if (led == NO_LED) {
                continue;
            }
            enum glow glow = pgm_read_byte(&glowmap[layer][row][col]);
            keypos_t  key  = { .row = row, .col = col };
            if (is_mode_key(keymap_key_to_keycode(layer, key))) {
                if (mac_mode) {
                    glow = GLOW_WHITE;
                }
                set_led_hsv(led, pulse(glow_palette[glow]));
            } else {
                set_led_hsv(led, glow_palette[glow]);
            }
        }
    }
}

bool rgb_matrix_indicators_user(void) {
    if (rawhid_state.rgb_control) {
        return false;
    }

    uint8_t layer = get_highest_layer(layer_state);
    if (layer < ARRAY_SIZE(glowmap)) {
        paint_layer(layer);
    }

    return true;
}
