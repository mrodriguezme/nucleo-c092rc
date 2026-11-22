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

// ----------------------------------------------------------------------------------------
// RCC MMIO addresses
// ----------------------------------------------------------------------------------------
constexpr uintptr_t RCC_BASE	  = 0x40021000;
constexpr uintptr_t RCC_CR	  = RCC_BASE + 0x00;
constexpr uintptr_t RCC_ICSCR	  = RCC_BASE + 0x04;
constexpr uintptr_t RCC_CFGR	  = RCC_BASE + 0x08;
constexpr uintptr_t RCC_CRRCR	  = RCC_BASE + 0x14;
constexpr uintptr_t RCC_CIER	  = RCC_BASE + 0x18;
constexpr uintptr_t RCC_CIFR	  = RCC_BASE + 0x1C;
constexpr uintptr_t RCC_CICR	  = RCC_BASE + 0x20;
constexpr uintptr_t RCC_IOPRSTR	  = RCC_BASE + 0x24;
constexpr uintptr_t RCC_AHBRSTR	  = RCC_BASE + 0x28;
constexpr uintptr_t RCC_APBRSTR1  = RCC_BASE + 0x2C;
constexpr uintptr_t RCC_APBRSTR2  = RCC_BASE + 0x30;
constexpr uintptr_t RCC_IOPENR	  = RCC_BASE + 0x34;
constexpr uintptr_t RCC_AHBENR	  = RCC_BASE + 0x38;
constexpr uintptr_t RCC_APBENR1	  = RCC_BASE + 0x3C;
constexpr uintptr_t RCC_APBENR2	  = RCC_BASE + 0x40;
constexpr uintptr_t RCC_IOPSMENR  = RCC_BASE + 0x44;
constexpr uintptr_t RCC_AHBSMENR  = RCC_BASE + 0x48;
constexpr uintptr_t RCC_APBSMENR1 = RCC_BASE + 0x4C;
constexpr uintptr_t RCC_APBSMENR2 = RCC_BASE + 0x50;
constexpr uintptr_t RCC_CCIPR	  = RCC_BASE + 0x54;
constexpr uintptr_t RCC_CCIPR2	  = RCC_BASE + 0x58;
constexpr uintptr_t RCC_CSR1	  = RCC_BASE + 0x5C;
constexpr uintptr_t RCC_CSR2	  = RCC_BASE + 0x60;

// ----------------------------------------------------------------------------------------
// RCC I/O port reset register (RCC_IOPRSTR) register layout
// ----------------------------------------------------------------------------------------
constexpr uint32_t RCC_IOPRSTR_GPIOARST = 1 << 0;
constexpr uint32_t RCC_IOPRSTR_GPIOBRST = 1 << 1;
constexpr uint32_t RCC_IOPRSTR_GPIOCRST = 1 << 2;
constexpr uint32_t RCC_IOPRSTR_GPIOFRST = 1 << 5;

// ----------------------------------------------------------------------------------------
// RCC I/O port clock enable register (RCC_IOPENR) register layout
// ----------------------------------------------------------------------------------------
constexpr uint32_t RCC_IOPENR_GPIOAEN = 1 << 0;
constexpr uint32_t RCC_IOPENR_GPIOBEN = 1 << 1;
constexpr uint32_t RCC_IOPENR_GPIOCEN = 1 << 2;
constexpr uint32_t RCC_IOPENR_GPIOFEN = 1 << 5;
