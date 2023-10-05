/* Copyright 2021 gtips
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

#include "print.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _LOWER,
    _DEVEL,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(// TODO: temp unmute not working
     // mute audio      temp unmute     toggle mute     ?               layer switch
        KC_MUTE,        KC_LCTL|KC_SPC, LAG(KC_K),      _______,        TO(_LOWER)
    ),
    /* Next (not sure why it's called LOWER?) */
    [_LOWER] = LAYOUT(
     // toggle RGB      cut             copy            paste           layer switch
        RGB_TOG,        LCTL(KC_X),     LCTL(KC_C),     LCTL(KC_V),     TO(_DEVEL)
    ),
    /* VS debugging shortcuts */
    [_DEVEL] = LAYOUT(
     // continue        toggle bkpt     step in         jump to PC      layer switch
        KC_F5,          KC_F9,          KC_F11,         LALT(KC_PAST),  TO(_BASE)
    ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    // Base                     volume down     volume up
    [_BASE]  = { ENCODER_CCW_CW(KC_VOLD,        KC_VOLU) },
    // Next                     mouse wheel up  mouse wheel dn
    [_LOWER] = { ENCODER_CCW_CW(KC_WH_U,        KC_WH_D) },
    // VS debug shortcuts       step out        step over
    [_DEVEL] = { ENCODER_CCW_CW(LSFT(KC_F11),   KC_F10) },
};
#endif

#if !defined(ENCODER_MAP_ENABLE)
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (IS_LAYER_ON(_LOWER)) {
          tap_code16((clockwise == true) ? KC_WH_D : KC_WH_U);
        } else {
          tap_code((clockwise == true) ? KC_VOLD : KC_VOLU);
        }
    }
    return true;
}
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _BASE:
    default: //  for any other layers, or the default layer
        print("BASE layer\n");
        rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
        rgblight_sethsv(HSV_TEAL);
        break;
    case _LOWER:
        print("LOWER layer\n");
        rgblight_mode(RGBLIGHT_MODE_RAINBOW_MOOD);
        break;
    case _DEVEL:
        print("DEVEL layer\n");
        rgblight_mode(RGBLIGHT_MODE_BREATHING);
        rgblight_sethsv(HSV_YELLOW);
        break;
    }
  return state;
}

