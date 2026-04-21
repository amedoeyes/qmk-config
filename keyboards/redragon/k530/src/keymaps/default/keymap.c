#include QMK_KEYBOARD_H

typedef enum {
    L_BASE,
    L_FN,
    L_SYSTEM,
    L_MOUSE,
    L_MOUSE_WHEEL,
    L_RGB,
    L_QMK,
    L_LNAV,
} layer;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LOSS:
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
        case KC_ROSS:
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [L_BASE] = LAYOUT_60_ansi(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,  //
                              KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, //
                              KC_ESC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,        //
                              KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,          //
                              KC_LCTL, KC_LOSS, KC_LALT, KC_SPC, KC_RALT, KC_ROSS, KC_RCTL, MO(L_LNAV)),

    [L_FN] = LAYOUT_60_ansi(XXXXXXX, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, XXXXXXX,                    //
                            XXXXXXX, XXXXXXX, XXXXXXX, KC_END, XXXXXXX, XXXXXXX, XXXXXXX, KC_PGUP, KC_INS, XXXXXXX, KC_PSCR, XXXXXXX, XXXXXXX, XXXXXXX, //
                            XXXXXXX, XXXXXXX, KC_HOME, KC_PGDN, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, XXXXXXX, XXXXXXX, XXXXXXX,          //
                            XXXXXXX, XXXXXXX, KC_DEL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  //
                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MO(L_LNAV)),

    [L_SYSTEM] = LAYOUT_60_ansi(XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU, KC_F20, KC_BRID, KC_BRIU, XXXXXXX, XXXXXXX, KC_MRWD, KC_MFFD, KC_MPRV, KC_MNXT, KC_MPLY,  //
                                XXXXXXX, XXXXXXX, KC_WAKE, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, //
                                XXXXXXX, XXXXXXX, KC_SLEP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          //
                                XXXXXXX, XXXXXXX, KC_PWR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    //
                                XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MO(L_LNAV)),

    [L_MOUSE] = LAYOUT_60_ansi(XXXXXXX, MS_ACL0, MS_ACL1, MS_ACL2, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, //
                               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, //
                               XXXXXXX, XXXXXXX, MS_BTN1, MS_BTN3, MS_BTN2, XXXXXXX, MS_LEFT, MS_DOWN, MS_UP, MS_RGHT, XXXXXXX, XXXXXXX, XXXXXXX,            //
                               KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT,                   //
                               KC_LCTL, XXXXXXX, KC_LALT, MO(L_MOUSE_WHEEL), KC_RALT, XXXXXXX, KC_RCTL, MO(L_LNAV)),

    [L_MOUSE_WHEEL] = LAYOUT_60_ansi(XXXXXXX, MS_ACL0, MS_ACL1, MS_ACL2, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, //
                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, //
                                     XXXXXXX, XXXXXXX, MS_BTN1, MS_BTN3, MS_BTN2, XXXXXXX, MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR, XXXXXXX, XXXXXXX, XXXXXXX,          //
                                     KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT,                   //
                                     KC_LCTL, XXXXXXX, KC_LALT, XXXXXXX, KC_RALT, XXXXXXX, KC_RCTL, MO(L_LNAV)),

    [L_RGB] = LAYOUT_60_ansi(RM_RST, RM_VALD, RM_VALU, RM_SATD, RM_SATU, RM_SPDD, RM_SPDU, RM_HUED, RM_HUEU, XXXXXXX, XXXXXXX, RM_PREV, RM_NEXT, RM_TOGG,  //
                             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, //
                             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          //
                             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   //
                             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MO(L_LNAV)),

    [L_QMK] = LAYOUT_60_ansi(XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, //
                             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_RBT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  //
                             XXXXXXX, XXXXXXX, XXXXXXX, DB_TOGG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          //
                             XXXXXXX, XXXXXXX, XXXXXXX, EE_CLR, XXXXXXX, QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    //
                             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MO(L_LNAV)),

    [L_LNAV] = LAYOUT_60_ansi(XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     //
                              XXXXXXX, TO(L_QMK), XXXXXXX, XXXXXXX, TO(L_RGB), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, //
                              TO(L_BASE), XXXXXXX, TO(L_SYSTEM), XXXXXXX, TO(L_FN), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     //
                              XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO(L_MOUSE), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   //
                              XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),
};
