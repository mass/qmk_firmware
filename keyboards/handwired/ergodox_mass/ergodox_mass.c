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
#include "ergodox_mass.h"

// Forward declarations
void mass_led_on(uint8_t idx);
void mass_led_off(uint8_t idx);
void mass_led_toggle(uint8_t idx);

// Pin index in PORTF for each of the LEDs
uint8_t LED_MASK[5] = { (1<<0), (1<<1), (1<<4), (1<<5), (1<<6) };

uint32_t scanCount = 0;

void matrix_init_kb(void) {
  // Set all LEDs as outputs & initialize them to off
  for (uint8_t i=0; i < 5; i++) {
    DDRF |= LED_MASK[i];
    mass_led_off(i);
  }

  // Set LEDs 0-2 based on default layer state
  mass_led_on(biton32(default_layer_state));

  matrix_init_user();
}

void matrix_scan_kb(void) {
  ++scanCount;

  // Blink LED4
  if (scanCount % 1000 == 0)
    mass_led_toggle(4);

  matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  //uprintf("RECORD %u\n", keycode);
  return process_record_user(keycode, record);
}

uint32_t layer_state_set_kb(uint32_t state) {
  //uprintf("LAYER %d\n", state);

  // Set LEDs 0-2 based on layer state
  for (uint8_t i=0; i < 3; i++)
    mass_led_off(i);
  mass_led_on(biton32(state));

  return state;
}

void mass_led_on(uint8_t idx) {
  if (idx >= 5) return;
  PORTF |= LED_MASK[idx];
}

void mass_led_off(uint8_t idx) {
  if (idx >= 5) return;
  PORTF &= ~LED_MASK[idx];
}

void mass_led_toggle(uint8_t idx) {
  if (idx >= 5) return;
  PORTF ^= LED_MASK[idx];
}
