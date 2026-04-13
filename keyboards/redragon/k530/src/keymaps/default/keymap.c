#include QMK_KEYBOARD_H

typedef enum {
    L_BASE,
    L_FN,
    L_MOUSE,
    L_RGB,
} layer;

typedef enum {
    KC_LRESET = SAFE_RANGE,
    KC_LGUI_OR_F13,
    KC_RGUI_OR_F13,
    RM_RST,
} keycode;

typedef enum {
    TD_LAYERS,
} tap_dance;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LRESET:
            layer_move(L_BASE);
            return false;
        case KC_LGUI_OR_F13:
            if (record->event.pressed) {
                if (detected_host_os() == OS_WINDOWS) {
                    register_code(KC_F13);
                } else {
                    register_code(KC_LGUI);
                }
            } else {
                if (detected_host_os() == OS_WINDOWS) {
                    unregister_code(KC_F13);
                } else {
                    unregister_code(KC_LGUI);
                }
            }
            return false;
        case KC_RGUI_OR_F13:
            if (record->event.pressed) {
                if (detected_host_os() == OS_WINDOWS) {
                    register_code(KC_F13);
                } else {
                    register_code(KC_RGUI);
                }
            } else {
                if (detected_host_os() == OS_WINDOWS) {
                    unregister_code(KC_F13);
                } else {
                    unregister_code(KC_RGUI);
                }
            }
            return false;
        case RM_RST:
            if (record->event.pressed) {
                rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
                rgb_matrix_sethsv_noeeprom(RGB_MATRIX_DEFAULT_HUE, RGB_MATRIX_DEFAULT_SAT, RGB_MATRIX_DEFAULT_VAL);
                rgb_matrix_set_speed(RGB_MATRIX_DEFAULT_SPD);
            }
            return false;
        default:
            return true;
    }
}

void td_layers_finished(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            layer_move(L_FN);
            break;
        case 2:
            layer_move(L_MOUSE);
            break;
        case 3:
            layer_move(L_RGB);
            break;
    }
}

const key_override_t *key_overrides[] = {
    &ko_make_basic(MOD_MASK_SHIFT, MS_LEFT, MS_WHLL),
    &ko_make_basic(MOD_MASK_SHIFT, MS_DOWN, MS_WHLD),
    &ko_make_basic(MOD_MASK_SHIFT, MS_UP, MS_WHLU),
    &ko_make_basic(MOD_MASK_SHIFT, MS_RGHT, MS_WHLR),
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_LAYERS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_layers_finished, NULL),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [L_BASE] = LAYOUT_60_ansi(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,  //
                              KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, //
                              KC_ESC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,        //
                              KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,          //
                              KC_LCTL, KC_LGUI_OR_F13, KC_LALT, KC_SPC, KC_RALT, KC_RGUI_OR_F13, KC_RCTL, TD(TD_LAYERS)),

    [L_FN] = LAYOUT_60_ansi(_______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, _______,                     //
                            _______, _______, _______, _______, _______, _______, _______, KC_PGUP, KC_INS, _______, KC_PSCR, _______, _______, _______, //
                            KC_LRESET, _______, _______, KC_PGDN, _______, _______, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, _______, _______, _______,         //
                            _______, _______, KC_DEL, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   //
                            _______, _______, _______, _______, _______, _______, MO(2), _______),

    [L_MOUSE] = LAYOUT_60_ansi(_______, MS_ACL0, MS_ACL1, MS_ACL2, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, //
                               _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, //
                               KC_LRESET, _______, MS_BTN1, MS_BTN3, MS_BTN2, _______, MS_LEFT, MS_DOWN, MS_UP, MS_RGHT, _______, _______, _______,          //
                               _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   //
                               _______, _______, _______, _______, _______, _______, _______, _______),

    [L_RGB] = LAYOUT_60_ansi(QK_BOOT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_PREV, RM_NEXT, RM_TOGG, //
                             _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_SPDD, RM_SPDU, RM_HUED, RM_HUEU, RM_RST,  //
                             KC_LRESET, _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_SATD, RM_SATU, _______,        //
                             _______, _______, _______, _______, _______, _______, _______, _______, _______, RM_VALD, RM_VALU, _______,                   //
                             _______, _______, _______, _______, _______, _______, _______, _______),
};
