/* Copyright 2018 Andrew Mass
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

/**
 * Misc Info
 *
 * - KC_NO for noop
 * - KC_TRNS for transparent (go to lower layer)
 * - *LAYER*: Momentary
 * - ^LAYER^: Toggle
 */

/**
 * TODO
 *
 * - More symbol layer keys
 * - Use extra thumb cluster keys
 */

// Layers
#define BASE 0
#define SYMB 1
#define NAVG 2

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
   CUSTOM = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   `    |   1  |   2  |   3  |   4  |   5  |      |           |      |   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   Q  |   W  |   E  |   R  |   T  |^SYMB^|           |^NAVG^|   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |   '    |
 * |--------+------+------+------+------+------| Hyper|           | Meh  |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   /  | RShift |
 * |--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------|
 * |        |      |      |*NAVG*|*SYMB*|                                       |*SYMB*|*NAVG*|      |      |        |
 * `------------------------------------'                                       `------------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |  Del |      |       |  GUI | Esc  |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       |      |      |      |
 *                                 | Space| Back |------|       |------| Cntl |Enter |
 *                                 |      |      | End  |       |  Tab |      |      |
 *                          ,------+------+-------------|       |--------------------+------.
 *                          |             |             |       |             |             |
 *                          `---------------------------'       `---------------------------'
 */

[BASE] = LAYOUT (
  KC_GRV,         KC_1,         KC_2,   KC_3,    KC_4,   KC_5,   KC_NO,
  KC_NO,          KC_Q,         KC_W,   KC_E,    KC_R,   KC_T,   TG(SYMB),
  KC_NO,          KC_A,         KC_S,   KC_D,    KC_F,   KC_G,
  KC_LSFT,        KC_Z,         KC_X,   KC_C,    KC_V,   KC_B,   ALL_T(KC_NO),
  KC_NO,          KC_NO,        KC_NO,  MO(NAVG),MO(SYMB),

                                                 KC_DELT,KC_NO,
                                                         KC_HOME,
                                        KC_SPC,  KC_BSPC,KC_END,
                                        KC_NO,           KC_NO,

 KC_NO,       KC_6,   KC_7,    KC_8,    KC_9,   KC_0,             KC_MINS,
 TG(NAVG),    KC_Y,   KC_U,    KC_I,    KC_O,   KC_P,             KC_BSLS,
              KC_H,   KC_J,    KC_K,    KC_L,   KC_SCLN,          KC_QUOT,
 MEH_T(KC_NO),KC_N,   KC_M,    KC_COMM, KC_DOT, KC_SLSH,          KC_RSFT,
                      MO(SYMB),MO(NAVG),KC_NO,  KC_NO,            KC_NO,

 KC_LGUI,        KC_ESC,
 KC_NO,
 KC_TAB,KC_RCTL, KC_ENT,
 KC_NO,          KC_NO
),

/* Keymap 1: Symbol Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |   <  |   >  |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |    +   |      |      |   {  |   }  |      |^SYMB^|           |^NAVG^|   7  |   8  |   9  |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |    =   |      |      |   (  |   )  |      |------|           |------|   4  |   5  |   6  |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | LShift |      |      |   [  |   ]  |      |      |           |   0  |   1  |   2  |   3  |      |      | RShift |
 * |--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------|
 * |        |      |      |*NAVG*|*SYMB*|                                       |*SYMB*|*NAVG*|      |      |        |
 * `------------------------------------'                                       `------------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |  Del |      |       |  GUI | Esc  |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       |      |      |      |
 *                                 | Space| Back |------|       |------| Cntl |Enter |
 *                                 |      |      | End  |       |  Tab |      |      |
 *                          ,------+------+-------------|       |--------------------+------.
 *                          |             |             |       |             |             |
 *                          `---------------------------'       `---------------------------'
 */

[SYMB] = LAYOUT (
  KC_NO,   KC_NO,   KC_NO,   KC_LABK, KC_RABK, KC_NO,   KC_NO,
  KC_PLUS, KC_NO,   KC_NO,   KC_LCBR, KC_RCBR, KC_NO,   KC_TRNS,
  KC_EQL,  KC_NO,   KC_NO,   KC_LPRN, KC_RPRN, KC_NO,
  KC_TRNS, KC_NO,   KC_NO,   KC_LBRC, KC_RBRC, KC_NO,   KC_NO,
  KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_TRNS,

                                      KC_TRNS, KC_TRNS,
                                               KC_TRNS,
                             KC_TRNS, KC_TRNS, KC_TRNS,
                             KC_TRNS,          KC_TRNS,

  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  KC_TRNS, KC_7,    KC_8,    KC_9,    KC_NO,   KC_NO,   KC_NO,
           KC_4,    KC_5,    KC_6,    KC_NO,   KC_NO,   KC_NO,
  KC_0,    KC_1,    KC_2,    KC_3,    KC_NO,   KC_NO,   KC_TRNS,
                    KC_TRNS, KC_TRNS, KC_NO,   KC_NO,   KC_NO,

  KC_TRNS, KC_TRNS,
  KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS,          KC_TRNS
),

/* Keymap 2: Navigation Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |^SYMB^|           |^NAVG^|      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MSE_LF|MSE_DN|MSE_UP|MSE_RT|------|           |------| LEFT | DOWN |  UP  | RGHT |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |MS_BT1|MS_BT2|MS_AC0|MS_AC1| MS_AC2 |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |*NAVG*|*SYMB*|                                       |*SYMB*|*NAVG*|      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |  Del |      |       |  GUI | Esc  |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       |      |      |      |
 *                                 | Space| Back |------|       |------| Cntl |Enter |
 *                                 |      |      | End  |       |  Tab |      |      |
 *                          ,------+------+-------------|       |--------------------+------.
 *                          |             |             |       |             |             |
 *                          `---------------------------'       `---------------------------'
 */

[NAVG] = LAYOUT (
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_TRNS,
  KC_NO,   KC_NO,   KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R,
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_TRNS,

                                      KC_TRNS, KC_TRNS,
                                               KC_TRNS,
                             KC_TRNS, KC_TRNS, KC_TRNS,
                             KC_TRNS,          KC_TRNS,

  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  KC_TRNS, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
           KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,   KC_NO,
  KC_NO,   KC_NO,   KC_BTN1, KC_BTN2, KC_ACL0, KC_ACL1, KC_ACL2,
                    KC_TRNS, KC_TRNS, KC_NO,   KC_NO,   KC_NO,

  KC_TRNS, KC_TRNS,
  KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS,          KC_TRNS
),

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}

void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

/* Keymap x: Empty Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |^SYMB^|           |^NAVG^|      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------|
 * |        |      |      |*NAVG*|*SYMB*|                                       |*SYMB*|*NAVG*|      |      |        |
 * `------------------------------------'                                       `------------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |  Del |      |       |  GUI | Esc  |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       |      |      |      |
 *                                 | Space| Back |------|       |------| Cntl |Enter |
 *                                 |      |      | End  |       |  Tab |      |      |
 *                          ,------+------+-------------|       |--------------------+------.
 *                          |             |             |       |             |             |
 *                          `---------------------------'       `---------------------------'
 */

/*
[    ] = LAYOUT (
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_TRNS,
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_TRNS,

                                      KC_TRNS, KC_TRNS,
                                               KC_TRNS,
                             KC_TRNS, KC_TRNS, KC_TRNS,
                             KC_TRNS,          KC_TRNS,

  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  KC_TRNS, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
           KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                    KC_TRNS, KC_TRNS, KC_NO,   KC_NO,   KC_NO,

  KC_TRNS, KC_TRNS,
  KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS,          KC_TRNS
),
*/
