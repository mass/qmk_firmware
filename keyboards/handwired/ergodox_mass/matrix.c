/*
Copyright 2012-2018 Jun Wako, Jack Humbert, Yiancar

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdint.h>
#include <stdbool.h>
#include "matrix.h"
#include "quantum.h"

static matrix_row_t matrix[MATRIX_ROWS];

typedef enum {
  R0 = 0,
  R1 = 1,
  R2 = 2,
  L0 = 3,
  L1 = 4,
  L2 = 5
} GPIOChip_t;

// GPIO Expander Control Bytes
#define GPX_READ  0b01000001
#define GPX_WRITE 0b01000000

// GPIO Expander Register Addresses
#define GPX_IODIR   0x00
#define GPX_IPOL    0x02
#define GPX_GPINTEN 0x04
#define GPX_DEFVAL  0x06
#define GPX_INTCON  0x08
#define GPX_IOCON   0x0A
#define GPX_GPPU    0x0C
#define GPX_INTF    0x0E
#define GPX_INTCAP  0x10
#define GPX_GPIO    0x12
#define GPX_OLAT    0x14

// Forward declarations
void spi_init(void);
uint8_t spi_send(uint8_t byte);
void spi_start(GPIOChip_t idx);
void spi_end(GPIOChip_t idx);
uint16_t gpx_read_reg(GPIOChip_t idx, uint8_t addr);
void gpx_write_reg(GPIOChip_t idx, uint8_t addr, uint16_t val);

// Define weak empty shells for these functions in case they don't get defined elsewhere

__attribute__ ((weak))
void matrix_init_quantum(void) {
  matrix_init_kb();
}

__attribute__ ((weak))
void matrix_scan_quantum(void) {
  matrix_scan_kb();
}

__attribute__ ((weak))
void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
  matrix_scan_user();
}

__attribute__ ((weak))
void matrix_init_user(void) {
}

__attribute__ ((weak))
void matrix_scan_user(void) {
}

// Define matrix functions

inline
uint8_t matrix_rows(void) {
  return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void) {
  return MATRIX_COLS;
}

inline
void matrix_setup(void) {
}

inline
bool matrix_is_modified(void) {
  return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col) {
  return (matrix[row] & ((matrix_row_t)1<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row) {
  return matrix[row];
}

inline
void matrix_print(void) {
}

inline
void matrix_power_up(void) {
}

inline
void matrix_power_down(void) {
}

/**
 * Initialize the matrix reading circuitry. Called once at the start
 */
void matrix_init(void) {
  // initialize matrix state: all keys off
  for (uint8_t i=0; i < MATRIX_ROWS; i++) {
    matrix[i] = 0;
  }

  // Initialize SPI bus
  spi_init();

  // Initialize all GPIO expanders with SPI messages
  for (int i = R0; i <= L2; ++i) {
    gpx_write_reg(i, GPX_IODIR, 0xFFFF); // Inputs
    gpx_write_reg(i, GPX_IPOL,  0xFFFF); // Flip polarity
    gpx_write_reg(i, GPX_IOCON, 0x0000); // Default config
    gpx_write_reg(i, GPX_GPPU,  0xFFFF); // Enable pullups
  }

  matrix_init_quantum();
}

/**
 * Read the matrix. Called continuously
 */
uint8_t matrix_scan(void) {
  for (int i = R0; i <= L2; ++i) {
    // A bank upper 8 bits, B bank lower 8 bits
    uint16_t gpio = gpx_read_reg(i, GPX_GPIO);

    // Even matrix rows are bank B, odd are bank A
    matrix[i*2] = gpio & 0xFF;
    matrix[i*2+1] = (gpio >> 8) & 0xFF;
  }

  matrix_scan_quantum();

  return 1;
}

/**
 * Initialize the SPI bus
 */
void spi_init(void) {
  // Initialize !CS pins to inactive outputs (high)
  DDRD |= (1<<6); // !CS_L0
  DDRD |= (1<<7); // !CS_L1
  DDRB |= (1<<4); // !CS_L2
  DDRB |= (1<<5); // !CS_R0
  DDRB |= (1<<6); // !CS_R1
  DDRC |= (1<<6); // !CS_R2
  PORTD |= (1<<6); // !CS_L0
  PORTD |= (1<<7); // !CS_L1
  PORTB |= (1<<4); // !CS_L2
  PORTB |= (1<<5); // !CS_R0
  PORTB |= (1<<6); // !CS_R1
  PORTC |= (1<<6); // !CS_R2

  // Disable power saving
  PRR0 &= ~(1<<2); // PRSPI

  // Initialize SPI pins
  DDRB |= (1<<0); // SS
  DDRB |= (1<<1); // SCK
  DDRB |= (1<<2); // SDO
  DDRB &= ~(1<<3); // SDI

  // Initialize SPCR register
  SPCR &= ~(1<<7); // SPIE (No interrupts)
  SPCR &= ~(1<<5); // DORD (MSB first)
  SPCR |= (1<<4); // MSTR (SPI Master)
  SPCR &= ~(1<<3); // CPOL (Mode 0)
  SPCR &= ~(1<<2); // CPHA (Mode 0)
  SPCR &= ~(0b11<<0); // SCK Frequency (F_osc / 4)

  // Enable SPI
  SPCR |= (1<<6); // SPE
}

/**
 * Send a single byte on the SPI bus and read the response
 */
uint8_t spi_send(uint8_t byte) {
  // Write byte to SPI register
  SPDR = byte;

  // Wait for transmission
  while (!(SPSR & (1<<7)));

  // Read response
  uint8_t resp = SPDR;

  return resp;
}

void spi_start(GPIOChip_t idx) {
  // Set !CS low
  switch (idx) {
    case L0: PORTD &= ~(1<<6); break;
    case L1: PORTD &= ~(1<<7); break;
    case L2: PORTB &= ~(1<<4); break;
    case R0: PORTB &= ~(1<<5); break;
    case R1: PORTB &= ~(1<<6); break;
    case R2: PORTC &= ~(1<<6); break;
  };
}

void spi_end(GPIOChip_t idx) {
  // Set !CS high
  switch (idx) {
    case L0: PORTD |= (1<<6); break;
    case L1: PORTD |= (1<<7); break;
    case L2: PORTB |= (1<<4); break;
    case R0: PORTB |= (1<<5); break;
    case R1: PORTB |= (1<<6); break;
    case R2: PORTC |= (1<<6); break;
  };
}

uint16_t gpx_read_reg(GPIOChip_t idx, uint8_t addr) {
  spi_start(idx);

  spi_send(GPX_READ);
  spi_send(addr);
  uint8_t resp1 = spi_send(0x00);
  uint8_t resp2 = spi_send(0x00);

  spi_end(idx);

  return resp1 << 8 | resp2;
}

void gpx_write_reg(GPIOChip_t idx, uint8_t addr, uint16_t val) {
  spi_start(idx);

  spi_send(GPX_WRITE);
  spi_send(addr);
  spi_send((val >> 8) & 0xFF);
  spi_send(val & 0xFF);

  spi_end(idx);
}
