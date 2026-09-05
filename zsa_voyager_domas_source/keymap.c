#include QMK_KEYBOARD_H
#include "version.h"
#define MOON_LED_LEVEL LED_LEVEL
#ifndef ZSA_SAFE_RANGE
#define ZSA_SAFE_RANGE SAFE_RANGE
#endif

enum custom_keycodes {
    RGB_SLD = ZSA_SAFE_RANGE,
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
#define NUM5_CLICK LT(1, KC_F17)
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
        _______,       _______,       _______,       _______,       RGB_VAD,       RGB_VAI,                 KC_MS_WH_UP,   LINE_START,    KC_UP,         LINE_END,      KC_PAGE_UP,       KC_F12,
        _______,       _______,       _______,       _______,       RGB_TOG,       RGB_MODE_FORWARD,        KC_MS_WH_DOWN, KC_LEFT,       KC_DOWN,       KC_RIGHT,      KC_PGDN,          _______,
        _______,       _______,       _______,       _______,       RGB_SLD,       TOGGLE_LAYER_COLOR,      _______,       WORD_LEFT,     _______,       WORD_RIGHT,    DELETE_LINE,      DELETE_WORD,
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

static bool     mac_mode;
static uint16_t left_app_command_held;
static uint16_t right_app_command_held;
static bool     app_switch_alt_held;

static void release_app_command(uint16_t *held) {
    if (*held) {
        unregister_code(*held);
        *held = 0;
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
    release_app_command(&left_app_command_held);
    release_app_command(&right_app_command_held);
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
    if (keycode == NUM5_CLICK) {
        return true;
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

static bool process_app_command(uint16_t *held, uint16_t tap_keycode, uint16_t windows_mod, uint16_t mac_mod, keyrecord_t *record) {
    if (record->tap.count) {
        if (record->event.pressed) {
            register_code16(tap_keycode);
        } else {
            unregister_code16(tap_keycode);
        }
        return false;
    }
    if (record->event.pressed) {
        *held = mac_mode ? mac_mod : windows_mod;
        register_code(*held);
    } else {
        release_app_command(held);
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    int8_t os_index = os_keycode_index(keycode);
    if (os_index >= 0) {
        return process_os_keycode(os_index, record);
    }

    switch (keycode) {
        case QK_MODS ... QK_MODS_MAX:
            // Mouse and consumer keys (volume, media) with modifiers work inconsistently across operating systems,
            // this makes sure that modifiers are always applied to the key that was pressed.
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
            return process_app_command(&left_app_command_held, KC_ESCAPE, KC_LEFT_CTRL, KC_LEFT_GUI, record);

        case APP_CMD_R:
            return process_app_command(&right_app_command_held, KC_SPACE, KC_RIGHT_CTRL, KC_RIGHT_GUI, record);

        case SWITCH_TAB:
            if (record->event.pressed) {
                // Windows switches apps on Alt+Tab, so the thumb's Ctrl is traded for Alt and kept
                // down until the thumb lifts. That is what lets repeated taps cycle the window list.
                if (!mac_mode && left_app_command_held && !app_switch_alt_held) {
                    release_app_command(&left_app_command_held);
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
                tap_code16(LSFT(os_keycode_for(os_keycode_index(LINE_START))));
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

        case RGB_SLD:
            if (record->event.pressed) {
                rgblight_mode(1);
            }
            return false;
    }
    return true;
}

extern rgb_config_t rgb_matrix_config;

RGB hsv_to_rgb_with_value(HSV hsv) {
    RGB   rgb = hsv_to_rgb(hsv);
    float f   = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
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
        GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_WHITE,   GLOW_WHITE,       GLOW_RED,     GLOW_CYAN,    GLOW_PURPLE,  GLOW_CYAN,    GLOW_PINK,    GLOW_LIME,
        GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_WHITE,   GLOW_WHITE,       GLOW_RED,     GLOW_PURPLE,  GLOW_PURPLE,  GLOW_PURPLE,  GLOW_PINK,    GLOW_OFF,
        GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_OFF,     GLOW_WHITE,   GLOW_WHITE,       GLOW_OFF,     GLOW_ORANGE,  GLOW_OFF,     GLOW_ORANGE,  GLOW_CYAN,    GLOW_ORANGE,
                                                                GLOW_OFF,     GLOW_OFF,         GLOW_OFF,     GLOW_GREEN
    ),
};

static void set_led_glow(uint8_t led, enum glow glow) {
    RGB rgb = hsv_to_rgb_with_value(glow_palette[glow]);
    rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
}

static void set_layer_color(uint8_t layer) {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t led = g_led_config.matrix_co[row][col];
            if (led == NO_LED) {
                continue;
            }
            set_led_glow(led, pgm_read_byte(&glowmap[layer][row][col]));
        }
    }
}

// The keys that send something different in Mac mode, plus the key that flips it.
// They show the mode: their glowmap colour on Windows, white on a Mac.
static const uint16_t mode_keys[] = { APP_CMD_L, APP_CMD_R, LGUI_T(KC_Z), RGUI_T(KC_SLASH), MAC_TOGGLE };

static bool is_mode_key(uint16_t keycode) {
    for (uint8_t i = 0; i < ARRAY_SIZE(mode_keys); i++) {
        if (mode_keys[i] == keycode) {
            return true;
        }
    }
    return false;
}

// Found by keycode, not by position, so moving a key in the keymap moves its
// light with it. Transparent cells on a layer are not mode keys and stay dark.
static void set_mode_keys_glow(uint8_t layer) {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            keypos_t key = { .row = row, .col = col };
            if (!is_mode_key(keymap_key_to_keycode(layer, key))) {
                continue;
            }
            uint8_t led = g_led_config.matrix_co[row][col];
            if (led != NO_LED) {
                set_led_glow(led, GLOW_WHITE);
            }
        }
    }
}

bool rgb_matrix_indicators_user(void) {
    if (rawhid_state.rgb_control) {
        return false;
    }

    uint8_t layer = get_highest_layer(layer_state);
    if (!keyboard_config.disable_layer_led && layer < ARRAY_SIZE(glowmap)) {
        set_layer_color(layer);
    } else if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
        rgb_matrix_set_color_all(0, 0, 0);
    }

    if (mac_mode) {
        set_mode_keys_glow(layer);
    }

    return true;
}
