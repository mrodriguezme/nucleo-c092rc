// SPDX-License-Identifier: MIT
//
// Copyright 2026 Michael Rodriguez <me@mrodriguez.me>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "hd44780.h"

#define NUM_COLS		 (16)
#define TIMEOUT_US_DEFAULT	 (50)
#define CLEAR_DISPLAY_TIMEOUT_US (2000)
#define SETUP_TIMEOUT_US	 (5000)

// clang-format off

static void (*const db_tbl[])(bool level) = {
	hd44780_db4_pin_set,
	hd44780_db5_pin_set,
	hd44780_db6_pin_set,
	hd44780_db7_pin_set
};

// clang-format on

enum hd44780_rs {
	HD44780_RS_COMMAND = 0,
	HD44780_RS_DATA	   = 1,
};

static void send_nibble(enum hd44780_rs rs, uint8_t nibble)
{
	// For 4-bit interface data, only four bus lines (DB4 to DB7) are used for transfer. Bus lines DB0 to DB3 are
	// disabled. The data transfer between the HD44780U and the MPU is completed after the 4-bit data has been
	// transferred twice.
	//
	// As for the order of data transfer, the four high order bits (for 8-bit operation, DB4 to DB7) are transferred
	// before the four low order bits (for 8-bit operation, DB0 to DB3).
	for (unsigned int bit = 0; bit < 4; ++bit)
		db_tbl[bit](nibble & (1 << bit));

	hd44780_rs_pin_set(rs);
	hd44780_delay_us(1);
	hd44780_e_pin_set(true);
	hd44780_delay_us(1);
	hd44780_e_pin_set(false);
}

static void send_byte(enum hd44780_rs rs, uint8_t byte, uint16_t timeout_us)
{
	send_nibble(rs, byte >> 4);
	send_nibble(rs, byte & 0x0F);

	hd44780_delay_us(timeout_us);
}

void hd44780_init(void)
{
	hd44780_e_pin_set(false);
	hd44780_delay_us(50000);

	for (int i = 0; i < 3; ++i) {
		send_nibble(HD44780_RS_COMMAND, 0b0011);
		hd44780_delay_us(SETUP_TIMEOUT_US);
	}

	send_nibble(HD44780_RS_COMMAND, 0b0010);
	hd44780_delay_us(150);

	send_byte(HD44780_RS_COMMAND, 0x28, TIMEOUT_US_DEFAULT);
	send_byte(HD44780_RS_COMMAND, 0x08, TIMEOUT_US_DEFAULT);
	send_byte(HD44780_RS_COMMAND, 0x01, CLEAR_DISPLAY_TIMEOUT_US);
	send_byte(HD44780_RS_COMMAND, 0x06, TIMEOUT_US_DEFAULT);
	send_byte(HD44780_RS_COMMAND, 0x0C, TIMEOUT_US_DEFAULT);
}

void hd44780_printf(const char *s, ...)
{
	va_list args;
	char buf[NUM_COLS + 1];

	va_start(args, s);
	int len = vsnprintf(buf, sizeof(buf), s, args);
	va_end(args);

	if (len < 0)
		return;

	if (len > NUM_COLS)
		len = NUM_COLS;

	for (int i = 0; i < len; ++i)
		send_byte(HD44780_RS_DATA, buf[i], TIMEOUT_US_DEFAULT);
}
