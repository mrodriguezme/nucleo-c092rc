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

#include <stdint.h>
#include "common/compiler_support.h"

extern void _start();
extern uint32_t __stack;

constexpr int VECTOR_MCU_BASE = 16;

#define VECTOR_TABLE_DATA(X) \
	X(0, &__stack)       \
	X(1, _start)

#define VECTOR_TABLE_CORE_ISRS(X) \
	X(nmi, 2)                 \
	X(hardfault, 3)           \
	X(svcall, 11)             \
	X(pendsv, 14)             \
	X(systick, 15)

#define VECTOR_TABLE_MCU_ISRS(X)          \
	X(wwdg, 0)                        \
	X(pvm, 1)                         \
	X(rtc, 2)                         \
	X(flash, 3)                       \
	X(rcc_crs, 4)                     \
	X(exti0_1, 5)                     \
	X(exti2_3, 6)                     \
	X(exti4_15, 7)                    \
	X(usb, 8)                         \
	X(dma1_channel1, 9)               \
	X(dma1_channel2_3, 10)            \
	X(dmamux_dma1_channel4_5_6_7, 11) \
	X(adc, 12)                        \
	X(tim1_brk_up_trg_com, 13)        \
	X(tim1_cc, 14)                    \
	X(tim2, 15)                       \
	X(tim3, 16)                       \
	X(tim14, 19)                      \
	X(tim15, 20)                      \
	X(tim16, 21)                      \
	X(tim17, 22)                      \
	X(i2c1, 23)                       \
	X(i2c2, 24)                       \
	X(spi1, 25)                       \
	X(spi2, 26)                       \
	X(usart1, 27)                     \
	X(usart2, 28)                     \
	X(usart3_usart4, 29)              \
	X(fdcan_it0, 30)                  \
	X(fdcan_it1, 31)

#define ISR_ALL(X)                \
	VECTOR_TABLE_CORE_ISRS(X) \
	VECTOR_TABLE_MCU_ISRS(X)

// clang-format off

#define ISR(fn)	       NOINLINE USED WEAK_SYMBOL SYMBOL_ALIAS("isr_unhandled") void fn()
#define X(name, index) ISR(isr_##name);
	ISR_ALL(X)
#undef X
#undef ISR

// clang-format on

void isr_unhandled()
{
	for (;;)
		;
}

// clang-format off

PLACE_IN_SECTION(".init")
USED const void *const __interrupt_vector[] = {
#define X(vector_index, data) [vector_index] = data,
	VECTOR_TABLE_DATA(X)
#undef X

#define X(name, index) [index] = isr_##name,
        VECTOR_TABLE_CORE_ISRS(X)
#undef X

#define X(name, irq) [VECTOR_MCU_BASE + (irq)] = isr_##name,
        VECTOR_TABLE_MCU_ISRS(X)
#undef X
};

// clang-format on
