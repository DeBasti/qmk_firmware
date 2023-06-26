/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include <stdio.h>
char wpm_str[10];

enum layers {
    _base = 0,
    _nav,
    _sym,
    _num,
    _son
};

enum custom_keycodes {
    winLeft,
    winRight,
    vscodeFormat
};


// Aliases for readability
// Left-hand home row mods
#define HOME_A LGUI_T(KC_A)
#define HOME_R LALT_T(KC_R)
#define HOME_S LCTL_T(KC_S)
#define HOME_T LSFT_T(KC_T)

// Right-hand home row mods
#define HOME_N RSFT_T(KC_N)
#define HOME_E RCTL_T(KC_E)
#define SON_I  LT(_son,KC_I)
#define HOME_O RGUI_T(KC_O)

// Auto Control
#define AC_C LT(10,KC_C) //Unused layer is used as placeholder and replaced by CTRL in "process_record_user" function
#define AC_V LT(10,KC_V)

//Default is tap preferred behavior, for shift it's hold preferred behavior
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_BSPC):
            // Force the dual-role key press to be handled as a modifier if any
            // other key was pressed while the mod-tap key is held down.
            return true;
        default:
            // Do not force the mod-tap key press to be handled as a modifier
            // if any other key was pressed while the mod-tap key is held down.
            return false;
    }
}

//Used for AutoControl C and V and macros and other shortcuts
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {          // Return true for normal processing of tap keycode
        case AC_C:
            if (!record->tap.count && record->event.pressed) {
                tap_code16(C(KC_C)); // Intercept hold function to send Ctrl-C
                return false;
            }
            return true;             // Return true for normal processing of tap keycode
        case AC_V:
            if (!record->tap.count && record->event.pressed) {
                tap_code16(C(KC_V)); // Intercept hold function to send Ctrl-V
                return false;
            }
            return true;             // Return true for normal processing of tap keycode
        case winLeft:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LGUI) SS_DOWN(X_LCTL) SS_TAP(X_LEFT) SS_UP(X_LGUI) SS_UP(X_LCTL));
                return false;
            }
        case winRight:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LGUI) SS_DOWN(X_LCTL) SS_TAP(X_RIGHT) SS_UP(X_LGUI) SS_UP(X_LCTL));
                return false;
            }
        case vscodeFormat:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LSFT) SS_DOWN(X_LALT) SS_TAP(X_F) SS_UP(X_LSFT) SS_UP(X_LALT));
                return false;
            }
    }
    return true;
}


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |   Q  |   W  |   F  |   P  |   B  |                              |   J  |   L  |   U  |   Y  |   '  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |   A  |   R  |   S  |   T  |   G  |                              |   M  |   N  |   E  |   I  |   O  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |   Z  |   X  |   C  |   D  |   V  |      |      |  |      |      |   K  |   H  |   ,  |   .  |   ?  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |win   | NAV/ | NUM  |Teams |  |      | SYM  | SHFT |win   |      |
 *                        |      |left  | SPC  | TAB  |Mute  |  |      | ENT  | BSPC |right |      |
 *                        `----------------------------------'  `----------------------------------'
 * I manually added the innermost thumb key into info.json but cannot find the correct position
 * therefore the keymap beneath does not match the real life but the keymap comment above does
 */
    [_base] = LAYOUT_split_3x5_4_encoder(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                                                   KC_J,    KC_L,    KC_U,    KC_Z,    LSFT(KC_NONUS_HASH),
        HOME_A,  HOME_R,  HOME_S,  HOME_T,  KC_G,                                                   KC_M,    HOME_N,  HOME_E,  SON_I,   HOME_O,
        KC_Y,    KC_X,    AC_C,    KC_D,    AC_V, XXXXXXX,                                 XXXXXXX, KC_K,    KC_H,    KC_COMM, KC_DOT,  LSFT(KC_MINUS),
                    LT(_nav,KC_SPC), LT(_num,KC_TAB), LCTL(KC_P6), winLeft,                        winRight, XXXXXXX, LT(_sym,KC_ENT),  LSFT_T(KC_BSPC)
    ),


/*
 * Nav Layer: Navigation
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      | Ins  | Del  | PGUP |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              | PSCR |  ←   |   ↑  |   →  |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      | Home |   ↓  | PGDN | End  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      | SPC  | TAB  |      |  |      | ESC  | TRNS |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */

    [_nav] = LAYOUT_split_3x5_4_encoder(
      XXXXXXX, LCTL(KC_P4), LCTL(KC_P5), LCTL(KC_P6), XXXXXXX,                                     XXXXXXX, KC_INS, KC_DEL, KC_PGUP, XXXXXXX,
      XXXXXXX, KC_P8, KC_P9, KC_VOLU, XXXXXXX,                                                     KC_PSCR, KC_LEFT, KC_UP, KC_RGHT, XXXXXXX,
      XXXXXXX, vscodeFormat, LCTL(KC_C), KC_VOLD, LCTL(KC_V), XXXXXXX,                    XXXXXXX, XXXXXXX, KC_HOME, KC_DOWN, KC_PGDN, KC_END,
                                    KC_SPC, KC_TAB, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, KC_ESC, KC_TRNS
    ),

/*
 * Sym Layer: Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  €   |  {   |  }   |  (   |  )   |                              |   [  |  ]   |  *   |  ~   |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  ^   |  +   |  =   |  -   |  $   |                              |   '  |  "   |  /   |  :   |  >   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |   ;  |  %   |  |   |  _   |  @   |      |      |  |      |      |   !  |  `   |  #   |  &   |  <   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |  ESC |   \  |      |  |      | ENT  | SHFT |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */

    [_sym] = LAYOUT_split_3x5_4_encoder(
        RALT(KC_E), RALT(KC_7), RALT(KC_0), LSFT(KC_8), LSFT(KC_9),                                                 RALT(KC_8), RALT(KC_9), LSFT(KC_RBRC), RALT(KC_RBRC), XXXXXXX,
        KC_GRAVE, KC_RBRC, LSFT(KC_0), KC_SLASH, LSFT(KC_4),                                                        LSFT(KC_NUHS), LSFT(KC_2), LSFT(KC_7), LSFT(KC_DOT), LSFT(KC_NUBS),
        LSFT(KC_COMMA), LSFT(KC_5), RALT(KC_NUBS), LSFT(KC_SLASH), RALT(KC_Q), XXXXXXX,                    XXXXXXX, LSFT(KC_1), LSFT(KC_EQUAL), KC_NUHS, LSFT(KC_6), KC_NUBS,
                                            KC_ESC, RALT(KC_MINUS), XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, KC_ENT, KC_TRNS
    ),

/*
  * Num Layer: Numbers and Function Keys
  *
  * ,-------------------------------------------.                              ,-------------------------------------------.
  * |        |      |  7   |  8   |  9   |      |                              |      |  F1  |  F2  |  F3  |  F4  |        |
  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
  * |        |   0  |  4   |  5   |  6   |      |                              |      |  F5  |  F6  |  F7  |  F8  |        |
  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
  * |        |      |  1   |  2   |  3   |      |      |      |  |      |      |      |  F9  | F10  | F11  | F12  |        |
  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
  *                        |      | SPC  | TAB  |      |      |  |      | TRNS | TRNS |      |      |
  *                        |      |      |      |      |      |  |      |      |      |      |      |
  *                        `----------------------------------'  `----------------------------------'
  */
     [_num] = LAYOUT_split_3x5_4_encoder(
       XXXXXXX, KC_7, KC_8, KC_9, XXXXXXX,                                     XXXXXXX, KC_F1, KC_F2,  KC_F3,  KC_F4, 
       KC_0,    KC_4, KC_5, KC_6, XXXXXXX,                                     XXXXXXX, KC_F5, KC_F6,  KC_F7,  KC_F8, 
       XXXXXXX, KC_1, KC_2, KC_3, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, KC_F9, KC_F10, KC_F11, KC_F12,
                  KC_SPC, KC_TAB, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, KC_TRNS, KC_TRNS
     ),

/*
 * Son Layer: German Umlaute
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |      |  Ü   |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |   Ä  |      |   ß  |      |      |                              |      |      |      |      |   Ö  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_son] = LAYOUT_split_3x5_4_encoder(
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                     XXXXXXX, XXXXXXX, KC_LBRC, XXXXXXX, XXXXXXX,
       KC_QUOTE, XXXXXXX, KC_MINUS, XXXXXXX, XXXXXXX,                                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_SCLN,
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                    KC_SPC, KC_TAB, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, KC_ESC, KC_TRNS
     ),

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT_split_3x5_4_encoder(
//       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
//                                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
//     ),
};

#ifdef ENCODER_ENABLE
    bool encoder_update_user(uint8_t index, bool clockwise) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
        return false;
    }
#endif