#include QMK_KEYBOARD_H

enum custom_keycodes {
    phrase
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {          // Return true for normal processing of tap keycode
        case phrase:
            if (record->event.pressed) {
                SEND_STRING("[Phrase goes here]");
                return false;
            }
        return true;             // Return true for normal processing of tap keycode
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
                 phrase, KC_F2, KC_F3,
                 KC_F5, LSFT(KC_F5), KC_F8,
        KC_MUTE, KC_F9, KC_F10,KC_F11
    )
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        tap_code_delay(KC_VOLU, 10);
    } else {
        tap_code_delay(KC_VOLD, 10);
    }
    return false;
}

