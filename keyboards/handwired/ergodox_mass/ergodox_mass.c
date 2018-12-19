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

uint32_t scanCount = 0;
uint8_t ledCycle = 0;

uint8_t LED_MASK[5] = { (1<<0), (1<<1), (1<<4), (1<<5), (1<<6) };

void mass_led_on(uint8_t idx);
void mass_led_off(uint8_t idx);
void mass_led_toggle(uint8_t idx);

void matrix_init_kb(void) {
  // Set all LEDs as outputs & initialize them to off
  for (uint8_t i=0; i < 5; i++) {
    DDRF |= LED_MASK[i];
    mass_led_off(i);
  }

  matrix_init_user();
}

void matrix_scan_kb(void) {
  ++scanCount;

  matrix_scan_user();

  if (scanCount % 100 == 0) {
    mass_led_toggle(ledCycle);
    if (++ledCycle == 5)
      ledCycle = 0;
  }
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  // put your per-action keyboard code here
  // runs for every action, just before processing by the firmware

  return process_record_user(keycode, record);
}

void led_set_kb(uint8_t usb_led) {
  // put your keyboard LED indicator (ex: Caps Lock LED) toggling code here

  led_set_user(usb_led);
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
