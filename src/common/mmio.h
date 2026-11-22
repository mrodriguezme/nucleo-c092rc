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
#include "compiler_support.h"

#define DEFINE_MMIO_HELPERS(bitwidth)                                                              \
	ALWAYS_INLINE static uint##bitwidth##_t mmio_read##bitwidth(uintptr_t addr)                \
	{                                                                                          \
		return *((volatile uint##bitwidth##_t *)addr);                                     \
	}                                                                                          \
                                                                                                   \
	ALWAYS_INLINE static void mmio_write##bitwidth(uintptr_t addr, uint##bitwidth##_t val)     \
	{                                                                                          \
		*((volatile uint##bitwidth##_t *)addr) = val;                                      \
	}                                                                                          \
                                                                                                   \
	ALWAYS_INLINE static void mmio_set##bitwidth(uintptr_t addr, uint##bitwidth##_t bits)      \
	{                                                                                          \
		uint##bitwidth##_t val = mmio_read##bitwidth(addr);                                \
		mmio_write##bitwidth(addr, val | bits);                                            \
	}                                                                                          \
                                                                                                   \
	ALWAYS_INLINE static void mmio_clr##bitwidth(uintptr_t addr, uint##bitwidth##_t bits)      \
	{                                                                                          \
		uint##bitwidth##_t val = mmio_read##bitwidth(addr);                                \
		mmio_write##bitwidth(addr, val & ~bits);                                           \
	}                                                                                          \
                                                                                                   \
	ALWAYS_INLINE static void mmio_rmw_mask##bitwidth(uintptr_t addr, uint##bitwidth##_t mask, \
							  uint##bitwidth##_t val)                  \
	{                                                                                          \
		uint##bitwidth##_t dst = mmio_read##bitwidth(addr);                                \
		dst &= ~mask;                                                                      \
		dst |= (uint##bitwidth##_t)(val << __builtin_ctz(mask)) & mask;                    \
		mmio_write##bitwidth(addr, dst);                                                   \
	}

DEFINE_MMIO_HELPERS(32);
DEFINE_MMIO_HELPERS(16);
DEFINE_MMIO_HELPERS(8);

#undef DEFINE_MMIO_HELPERS
