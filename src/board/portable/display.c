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

#include "display.h"
#include "config.h"
#include "mcu/gpio.h"
#include "mcu/util.h"

GPIO_PIN_DEFINE(DISPLAY_PIN_E, A, 0);
GPIO_PIN_DEFINE(DISPLAY_PIN_RS, A, 1);
GPIO_PIN_DEFINE(DISPLAY_PIN_DB4, A, 4);
GPIO_PIN_DEFINE(DISPLAY_PIN_DB5, B, 0);
GPIO_PIN_DEFINE(DISPLAY_PIN_DB6, C, 4);
GPIO_PIN_DEFINE(DISPLAY_PIN_DB7, C, 5);

#define pin_init_output(port, pin)                                                      \
	do {                                                                            \
		gpio_port_pin_mode_set((port), (pin), GPIO_MODER_MODE_OUTPUT);          \
                                                                                        \
		gpio_pin_output_type_set((port), (pin), GPIO_OTYPER_PUSH_PULL);         \
		gpio_pin_output_speed_set((port), (pin), GPIO_OSPEEDR_VERY_HIGH_SPEED); \
		gpio_pin_pupd_set((port), (pin), GPIO_PUPD_NO_PULL_UP_OR_PULL_DOWN);    \
                                                                                        \
		gpio_pin_set_low((port), (pin));                                        \
	} while (0)

#define pin_level_set(port, pin, level)                   \
	do {                                              \
		if ((level))                              \
			gpio_pin_set_high((port), (pin)); \
		else                                      \
			gpio_pin_set_low((port), (pin));  \
	} while (0)

static void gpio_init()
{
	pin_init_output(DISPLAY_PIN_E_PORT, DISPLAY_PIN_E_PIN);
	pin_init_output(DISPLAY_PIN_RS_PORT, DISPLAY_PIN_RS_PIN);
	pin_init_output(DISPLAY_PIN_DB4_PORT, DISPLAY_PIN_DB4_PIN);
	pin_init_output(DISPLAY_PIN_DB5_PORT, DISPLAY_PIN_DB5_PIN);
	pin_init_output(DISPLAY_PIN_DB6_PORT, DISPLAY_PIN_DB6_PIN);
	pin_init_output(DISPLAY_PIN_DB7_PORT, DISPLAY_PIN_DB7_PIN);
}

void hd44780_delay_us(uint16_t us)
{
	delay_us(BOARD_CPU_CLKFREQ_HZ, us);
}

void hd44780_rs_pin_set(bool level)
{
	pin_level_set(DISPLAY_PIN_RS_PORT, DISPLAY_PIN_RS_PIN, level);
}

void hd44780_e_pin_set(bool level)
{
	pin_level_set(DISPLAY_PIN_E_PORT, DISPLAY_PIN_E_PIN, level);
}

void hd44780_db4_pin_set(bool level)
{
	pin_level_set(DISPLAY_PIN_DB4_PORT, DISPLAY_PIN_DB4_PIN, level);
}

void hd44780_db5_pin_set(bool level)
{
	pin_level_set(DISPLAY_PIN_DB5_PORT, DISPLAY_PIN_DB5_PIN, level);
}

void hd44780_db6_pin_set(bool level)
{
	pin_level_set(DISPLAY_PIN_DB6_PORT, DISPLAY_PIN_DB6_PIN, level);
}

void hd44780_db7_pin_set(bool level)
{
	pin_level_set(DISPLAY_PIN_DB7_PORT, DISPLAY_PIN_DB7_PIN, level);
}

void board_portable_display_init()
{
	gpio_init();
}
