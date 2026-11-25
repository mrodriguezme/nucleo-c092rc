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

#pragma once

#include <stdint.h>

void hd44780_init(void);

// ----------------------------------------------------------------------------------------
// High level interfaces
// ----------------------------------------------------------------------------------------
void hd44780_printf(const char *s, ...);
void hd44780_puts(const char *s);

// ----------------------------------------------------------------------------------------
// Functions required to be supplied by application
// ----------------------------------------------------------------------------------------
extern void hd44780_delay_us(uint16_t us);

extern void hd44780_rs_pin_set(bool level);
extern void hd44780_e_pin_set(bool level);
extern void hd44780_db4_pin_set(bool level);
extern void hd44780_db5_pin_set(bool level);
extern void hd44780_db6_pin_set(bool level);
extern void hd44780_db7_pin_set(bool level);
