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

#include "common/mmio.h"
#include "common/util.h"

// ----------------------------------------------------------------------------------------
// GPIO MMIO addresses
// ----------------------------------------------------------------------------------------
#define GPIO_MODER(base)   ((base) + 0x00)
#define GPIO_OTYPER(base)  ((base) + 0x04)
#define GPIO_OSPEEDR(base) ((base) + 0x08)
#define GPIO_PUPDR(base)   ((base) + 0x0C)
#define GPIO_IDR(base)	   ((base) + 0x10)
#define GPIO_ODR(base)	   ((base) + 0x14)
#define GPIO_BSRR(base)	   ((base) + 0x18)
#define GPIO_LCKR(base)	   ((base) + 0x1C)
#define GPIO_AFRL(base)	   ((base) + 0x20)
#define GPIO_AFRH(base)	   ((base) + 0x24)
#define GPIO_BRR(base)	   ((base) + 0x28)

// ----------------------------------------------------------------------------------------
// GPIO helper macros
// ----------------------------------------------------------------------------------------
#define static_assert_port_valid(port)                                                                  \
	static_assert(((port) == GPIOA) || ((port) == GPIOB) || ((port) == GPIOC) || ((port) == GPIOF), \
		      "Port not valid");

#define static_assert_pin_valid(pin) static_assert((pin) <= 15, "Pin not valid (must be between 0..15)")

#define gpio_port_pin_mode_set(port, pin, mode)                                                          \
	do {                                                                                             \
		static_assert_port_valid((port));                                                        \
		static_assert_pin_valid((pin));                                                          \
                                                                                                         \
		static_assert(((mode) == GPIO_MODER_MODE_INPUT) || ((mode) == GPIO_MODER_MODE_OUTPUT) || \
				      ((mode) == GPIO_MODER_MODE_ALTERNATE_FUNCTION) ||                  \
				      ((mode) == GPIO_MODER_MODE_ANALOG),                                \
			      "Port mode not valid");                                                    \
                                                                                                         \
		mmio_rmw_mask32(GPIO_MODER((port)), 0x3 << ((pin) * 2), (mode));                         \
	} while (0)

#define gpio_pin_output_type_set(port, pin, ot)                         \
	do {                                                            \
		static_assert_port_valid((port));                       \
		static_assert_pin_valid((pin));                         \
                                                                        \
		mmio_rmw_mask32(GPIO_OTYPER((port)), 1 << (pin), (ot)); \
	} while (0)

#define gpio_pin_output_speed_set(port, pin, ospeed)                                                               \
	do {                                                                                                       \
		static_assert_port_valid((port));                                                                  \
		static_assert_pin_valid((pin));                                                                    \
                                                                                                                   \
		static_assert(((ospeed) == GPIO_OSPEEDR_VERY_LOW_SPEED) || ((ospeed) == GPIO_OSPEEDR_LOW_SPEED) || \
				      ((ospeed) == GPIO_OSPEEDR_HIGH_SPEED) ||                                     \
				      ((ospeed) == GPIO_OSPEEDR_VERY_HIGH_SPEED),                                  \
			      "Speed not valid");                                                                  \
                                                                                                                   \
		mmio_rmw_mask32(GPIO_OSPEEDR((port)), UINT32_C(0x3) << ((pin) * 2), (ospeed));                     \
	} while (0)

#define gpio_pin_pupd_set(port, pin, pupd)                                                                      \
	do {                                                                                                    \
		static_assert_port_valid((port));                                                               \
		static_assert_pin_valid((pin));                                                                 \
                                                                                                                \
		static_assert(((pupd) == GPIO_PUPD_NO_PULL_UP_OR_PULL_DOWN) || ((pupd) == GPIO_PUPD_PULL_UP) || \
				      ((pupd) == GPIO_PUPD_PULL_DOWN),                                          \
			      "Resistor type not valid");                                                       \
                                                                                                                \
		mmio_rmw_mask32(GPIO_PUPDR((port)), UINT32_C(0x3) << ((pin) * 2), (pupd));                      \
	} while (0)

#define gpio_pin_set_high(port, pin)                                   \
	do {                                                           \
		static_assert_port_valid((port));                      \
		static_assert_pin_valid((pin));                        \
                                                                       \
		mmio_write32(GPIO_BSRR((port)), UINT32_C(1) << (pin)); \
	} while (0)

#define gpio_pin_set_low(port, pin)                                           \
	do {                                                                  \
		static_assert_port_valid((port));                             \
		static_assert_pin_valid((pin));                               \
                                                                              \
		mmio_write32(GPIO_BSRR((port)), UINT32_C(1) << ((pin) + 16)); \
	} while (0)

#define GPIO_PIN_DEFINE(name, port, pin)              \
	constexpr uintptr_t name##_PORT = GPIO##port; \
	constexpr uint32_t name##_PIN	= (pin)

// ----------------------------------------------------------------------------------------
// Base addresses for each GPIO module
// ----------------------------------------------------------------------------------------
constexpr uintptr_t GPIOA = 0x50000000;
constexpr uintptr_t GPIOB = 0x50000400;
constexpr uintptr_t GPIOC = 0x50000800;
constexpr uintptr_t GPIOF = 0x50001400;

// ----------------------------------------------------------------------------------------
// GPIO port mode register (GPIOx_MODER) (x = A, B, C, F) register layout
//
// MODEy: Port x configuration for I/O y (y = 15 to 0)
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_MODER_MODE0  = GENMASK32(1, 0);
constexpr uint32_t GPIO_MODER_MODE1  = GENMASK32(3, 2);
constexpr uint32_t GPIO_MODER_MODE2  = GENMASK32(5, 4);
constexpr uint32_t GPIO_MODER_MODE3  = GENMASK32(7, 6);
constexpr uint32_t GPIO_MODER_MODE4  = GENMASK32(9, 8);
constexpr uint32_t GPIO_MODER_MODE5  = GENMASK32(11, 10);
constexpr uint32_t GPIO_MODER_MODE6  = GENMASK32(13, 12);
constexpr uint32_t GPIO_MODER_MODE7  = GENMASK32(15, 14);
constexpr uint32_t GPIO_MODER_MODE8  = GENMASK32(17, 16);
constexpr uint32_t GPIO_MODER_MODE9  = GENMASK32(19, 18);
constexpr uint32_t GPIO_MODER_MODE10 = GENMASK32(21, 20);
constexpr uint32_t GPIO_MODER_MODE11 = GENMASK32(23, 22);
constexpr uint32_t GPIO_MODER_MODE12 = GENMASK32(25, 24);
constexpr uint32_t GPIO_MODER_MODE13 = GENMASK32(27, 26);
constexpr uint32_t GPIO_MODER_MODE14 = GENMASK32(29, 28);
constexpr uint32_t GPIO_MODER_MODE15 = GENMASK32(31, 30);

// ----------------------------------------------------------------------------------------
// GPIO port mode register (GPIOx_MODER) (x = A, B, C, F) register values
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_MODER_MODE_INPUT	      = 0b00;
constexpr uint32_t GPIO_MODER_MODE_OUTPUT	      = 0b01;
constexpr uint32_t GPIO_MODER_MODE_ALTERNATE_FUNCTION = 0b10;
constexpr uint32_t GPIO_MODER_MODE_ANALOG	      = 0b11;

// ----------------------------------------------------------------------------------------
// GPIO port output type register (GPIOx_OTYPER) (x = A, B, C, F) register layout
//
// OTy: Port x configuration for I/O y (y = 15 to 0)
//
// - Bit clear: Output push-pull (reset state)
// - Bit set:   Output open-drain
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_OTYPER_OT0  = 1 << 0;
constexpr uint32_t GPIO_OTYPER_OT1  = 1 << 1;
constexpr uint32_t GPIO_OTYPER_OT2  = 1 << 2;
constexpr uint32_t GPIO_OTYPER_OT3  = 1 << 3;
constexpr uint32_t GPIO_OTYPER_OT4  = 1 << 4;
constexpr uint32_t GPIO_OTYPER_OT5  = 1 << 5;
constexpr uint32_t GPIO_OTYPER_OT6  = 1 << 6;
constexpr uint32_t GPIO_OTYPER_OT7  = 1 << 7;
constexpr uint32_t GPIO_OTYPER_OT8  = 1 << 8;
constexpr uint32_t GPIO_OTYPER_OT9  = 1 << 9;
constexpr uint32_t GPIO_OTYPER_OT10 = 1 << 10;
constexpr uint32_t GPIO_OTYPER_OT11 = 1 << 11;
constexpr uint32_t GPIO_OTYPER_OT12 = 1 << 12;
constexpr uint32_t GPIO_OTYPER_OT13 = 1 << 13;
constexpr uint32_t GPIO_OTYPER_OT14 = 1 << 14;
constexpr uint32_t GPIO_OTYPER_OT15 = 1 << 15;

constexpr uint32_t GPIO_OTYPER_PUSH_PULL  = 0;
constexpr uint32_t GPIO_OTYPER_OPEN_DRAIN = 1;

// ----------------------------------------------------------------------------------------
// GPIO port output speed register (GPIOx_OSPEEDR) (x = A, B, C, F)
//
// OSPEEDy: Port x configuration for I/O y (y = 15 to 0)
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_OSPEEDR_OSPEED0	 = GENMASK32(1, 0);
constexpr uint32_t GPIO_OSPEEDR_OSPEED1	 = GENMASK32(3, 2);
constexpr uint32_t GPIO_OSPEEDR_OSPEED2	 = GENMASK32(5, 4);
constexpr uint32_t GPIO_OSPEEDR_OSPEED3	 = GENMASK32(7, 6);
constexpr uint32_t GPIO_OSPEEDR_OSPEED4	 = GENMASK32(9, 8);
constexpr uint32_t GPIO_OSPEEDR_OSPEED5	 = GENMASK32(11, 10);
constexpr uint32_t GPIO_OSPEEDR_OSPEED6	 = GENMASK32(13, 12);
constexpr uint32_t GPIO_OSPEEDR_OSPEED7	 = GENMASK32(15, 14);
constexpr uint32_t GPIO_OSPEEDR_OSPEED8	 = GENMASK32(17, 16);
constexpr uint32_t GPIO_OSPEEDR_OSPEED9	 = GENMASK32(19, 18);
constexpr uint32_t GPIO_OSPEEDR_OSPEED10 = GENMASK32(21, 20);
constexpr uint32_t GPIO_OSPEEDR_OSPEED11 = GENMASK32(23, 22);
constexpr uint32_t GPIO_OSPEEDR_OSPEED12 = GENMASK32(25, 24);
constexpr uint32_t GPIO_OSPEEDR_OSPEED13 = GENMASK32(27, 26);
constexpr uint32_t GPIO_OSPEEDR_OSPEED14 = GENMASK32(29, 28);
constexpr uint32_t GPIO_OSPEEDR_OSPEED15 = GENMASK32(31, 30);

// ----------------------------------------------------------------------------------------
// GPIO port output speed register (GPIOx_OSPEEDR) (x = A, B, C, F) register values
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_OSPEEDR_VERY_LOW_SPEED	= 0b00;
constexpr uint32_t GPIO_OSPEEDR_LOW_SPEED	= 0b01;
constexpr uint32_t GPIO_OSPEEDR_HIGH_SPEED	= 0b10;
constexpr uint32_t GPIO_OSPEEDR_VERY_HIGH_SPEED = 0b11;

// ----------------------------------------------------------------------------------------
// GPIO port pull-up/pull-down register (GPIOx_PUPDR) (x = A, B, C, F) register layout
//
// PUPDy: Port x configuration I/O y (y = 15 to 0)
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_PUPDR_PUPD0  = GENMASK32(1, 0);
constexpr uint32_t GPIO_PUPDR_PUPD1  = GENMASK32(3, 2);
constexpr uint32_t GPIO_PUPDR_PUPD2  = GENMASK32(5, 4);
constexpr uint32_t GPIO_PUPDR_PUPD3  = GENMASK32(7, 6);
constexpr uint32_t GPIO_PUPDR_PUPD4  = GENMASK32(9, 8);
constexpr uint32_t GPIO_PUPDR_PUPD5  = GENMASK32(11, 10);
constexpr uint32_t GPIO_PUPDR_PUPD6  = GENMASK32(13, 12);
constexpr uint32_t GPIO_PUPDR_PUPD7  = GENMASK32(15, 14);
constexpr uint32_t GPIO_PUPDR_PUPD8  = GENMASK32(17, 16);
constexpr uint32_t GPIO_PUPDR_PUPD9  = GENMASK32(19, 18);
constexpr uint32_t GPIO_PUPDR_PUPD10 = GENMASK32(21, 20);
constexpr uint32_t GPIO_PUPDR_PUPD11 = GENMASK32(23, 22);
constexpr uint32_t GPIO_PUPDR_PUPD12 = GENMASK32(25, 24);
constexpr uint32_t GPIO_PUPDR_PUPD13 = GENMASK32(27, 26);
constexpr uint32_t GPIO_PUPDR_PUPD14 = GENMASK32(29, 28);
constexpr uint32_t GPIO_PUPDR_PUPD15 = GENMASK32(31, 30);

// ----------------------------------------------------------------------------------------
// GPIO port pull-up/pull-down register (GPIOx_PUPDR) (x = A, B, C, F) register values
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_PUPD_NO_PULL_UP_OR_PULL_DOWN = 0b00;
constexpr uint32_t GPIO_PUPD_PULL_UP		     = 0b01;
constexpr uint32_t GPIO_PUPD_PULL_DOWN		     = 0b10;

// ----------------------------------------------------------------------------------------
// GPIO port input data register (GPIOx_IDR) (x = A, B, C, F) register layout
//
// IDy: Port x input data I/O y (y = 15 to 0)
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_IDR_ID0	 = 1 << 0;
constexpr uint32_t GPIO_IDR_ID1	 = 1 << 1;
constexpr uint32_t GPIO_IDR_ID2	 = 1 << 2;
constexpr uint32_t GPIO_IDR_ID3	 = 1 << 3;
constexpr uint32_t GPIO_IDR_ID4	 = 1 << 4;
constexpr uint32_t GPIO_IDR_ID5	 = 1 << 5;
constexpr uint32_t GPIO_IDR_ID6	 = 1 << 6;
constexpr uint32_t GPIO_IDR_ID7	 = 1 << 7;
constexpr uint32_t GPIO_IDR_ID8	 = 1 << 8;
constexpr uint32_t GPIO_IDR_ID9	 = 1 << 9;
constexpr uint32_t GPIO_IDR_ID10 = 1 << 10;
constexpr uint32_t GPIO_IDR_ID11 = 1 << 11;
constexpr uint32_t GPIO_IDR_ID12 = 1 << 12;
constexpr uint32_t GPIO_IDR_ID13 = 1 << 13;
constexpr uint32_t GPIO_IDR_ID14 = 1 << 14;
constexpr uint32_t GPIO_IDR_ID15 = 1 << 15;

// ----------------------------------------------------------------------------------------
// GPIO port output data register (GPIOx_ODR) (x = A, B, C, F)
//
// ODy: Port output data I/O y (y = 15 to 0)
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_ODR_OD0	 = 1 << 0;
constexpr uint32_t GPIO_ODR_OD1	 = 1 << 1;
constexpr uint32_t GPIO_ODR_OD2	 = 1 << 2;
constexpr uint32_t GPIO_ODR_OD3	 = 1 << 3;
constexpr uint32_t GPIO_ODR_OD4	 = 1 << 4;
constexpr uint32_t GPIO_ODR_OD5	 = 1 << 5;
constexpr uint32_t GPIO_ODR_OD6	 = 1 << 6;
constexpr uint32_t GPIO_ODR_OD7	 = 1 << 7;
constexpr uint32_t GPIO_ODR_OD8	 = 1 << 8;
constexpr uint32_t GPIO_ODR_OD9	 = 1 << 9;
constexpr uint32_t GPIO_ODR_OD10 = 1 << 10;
constexpr uint32_t GPIO_ODR_OD11 = 1 << 11;
constexpr uint32_t GPIO_ODR_OD12 = 1 << 12;
constexpr uint32_t GPIO_ODR_OD13 = 1 << 13;
constexpr uint32_t GPIO_ODR_OD14 = 1 << 14;
constexpr uint32_t GPIO_ODR_OD15 = 1 << 15;

// ----------------------------------------------------------------------------------------
// GPIO port bit set/reset register (GPIOx_BSRR) (x = A, B, C, F)
//
// Bits 31:16 BRy: Port x reset I/O y (y = 15 to 0)
//   These bits are write-only. A read operation always returns 0x0000.
//     - 0: No action on the corresponding ODRx bit
//     - 1: Resets the corresponding ODRx bit
//
//   Note: If both BSx and BRx are set, BSx has priority.
//
// Bits 15:0 BSy: Port x set I/O y (y = 15 to 0)
//   These bits are write-only. A read operation always returns 0x0000.
//     - 0: No action on the corresponding ODRx bit
//     - 1: Sets the corresponding ODRx bit
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_BSRR_BS0  = 1 << 0;
constexpr uint32_t GPIO_BSRR_BS1  = 1 << 1;
constexpr uint32_t GPIO_BSRR_BS2  = 1 << 2;
constexpr uint32_t GPIO_BSRR_BS3  = 1 << 3;
constexpr uint32_t GPIO_BSRR_BS4  = 1 << 4;
constexpr uint32_t GPIO_BSRR_BS5  = 1 << 5;
constexpr uint32_t GPIO_BSRR_BS6  = 1 << 6;
constexpr uint32_t GPIO_BSRR_BS7  = 1 << 7;
constexpr uint32_t GPIO_BSRR_BS8  = 1 << 8;
constexpr uint32_t GPIO_BSRR_BS9  = 1 << 9;
constexpr uint32_t GPIO_BSRR_BS10 = 1 << 10;
constexpr uint32_t GPIO_BSRR_BS11 = 1 << 11;
constexpr uint32_t GPIO_BSRR_BS12 = 1 << 12;
constexpr uint32_t GPIO_BSRR_BS13 = 1 << 13;
constexpr uint32_t GPIO_BSRR_BS14 = 1 << 14;
constexpr uint32_t GPIO_BSRR_BS15 = 1 << 15;
constexpr uint32_t GPIO_BSRR_BR0  = 1 << 16;
constexpr uint32_t GPIO_BSRR_BR1  = 1 << 17;
constexpr uint32_t GPIO_BSRR_BR2  = 1 << 18;
constexpr uint32_t GPIO_BSRR_BR3  = 1 << 19;
constexpr uint32_t GPIO_BSRR_BR4  = 1 << 20;
constexpr uint32_t GPIO_BSRR_BR5  = 1 << 21;
constexpr uint32_t GPIO_BSRR_BR6  = 1 << 22;
constexpr uint32_t GPIO_BSRR_BR7  = 1 << 23;
constexpr uint32_t GPIO_BSRR_BR8  = 1 << 24;
constexpr uint32_t GPIO_BSRR_BR9  = 1 << 25;
constexpr uint32_t GPIO_BSRR_BR10 = 1 << 26;
constexpr uint32_t GPIO_BSRR_BR11 = 1 << 27;
constexpr uint32_t GPIO_BSRR_BR12 = 1 << 28;
constexpr uint32_t GPIO_BSRR_BR13 = 1 << 29;
constexpr uint32_t GPIO_BSRR_BR14 = 1 << 30;
constexpr uint32_t GPIO_BSRR_BR15 = UINT32_C(1) << 31;

// ----------------------------------------------------------------------------------------
// GPIO port configuration lock register (GPIOx_LCKR) (x = A, B, C, F)
//
// Bit 16 LCKK: Lock key
//   This bit can be read any time. It can only be modified using the lock key write sequence.
//     - 0: Port configuration lock key not active
//     - 1: Port configuration lock key active. The GPIOx_LCKR register is locked until the next MCU reset or peripheral
//          reset.
//
//   LOCK key write sequence:
//       - WR LCKR[16] = '1' + LCKR[15:0]
//       - WR LCKR[16] = '0' + LCKR[15:0]
//       - WR LCKR[16] = '1' + LCKR[15:0]
//       - RD LCKR
//       - RD LCKR[16] = '1' (this read operation is optional but it confirms that the lock is active)
//
//   Note: During the LOCK key write sequence, the value of LCK[15:0] must not change.
//         Any error in the lock sequence aborts the lock.
//         After the first lock sequence on any bit of the port, any read access on the LCKK bit returns '1' until the
//         next MCU reset or peripheral reset.
//
// Bits 15:0 LCK[15:0]: Port x lock I/O pin y (y = 15 to 0)
//   These bits are read/write but can only be written when the LCKK bit is 0.
//     - 0: Port configuration not locked
//     - 1: Port configuration locked
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_LCKR_LCK0  = 1 << 0;
constexpr uint32_t GPIO_LCKR_LCK1  = 1 << 1;
constexpr uint32_t GPIO_LCKR_LCK2  = 1 << 2;
constexpr uint32_t GPIO_LCKR_LCK3  = 1 << 3;
constexpr uint32_t GPIO_LCKR_LCK4  = 1 << 4;
constexpr uint32_t GPIO_LCKR_LCK5  = 1 << 5;
constexpr uint32_t GPIO_LCKR_LCK6  = 1 << 6;
constexpr uint32_t GPIO_LCKR_LCK7  = 1 << 7;
constexpr uint32_t GPIO_LCKR_LCK8  = 1 << 8;
constexpr uint32_t GPIO_LCKR_LCK9  = 1 << 9;
constexpr uint32_t GPIO_LCKR_LCK10 = 1 << 10;
constexpr uint32_t GPIO_LCKR_LCK11 = 1 << 11;
constexpr uint32_t GPIO_LCKR_LCK12 = 1 << 12;
constexpr uint32_t GPIO_LCKR_LCK13 = 1 << 13;
constexpr uint32_t GPIO_LCKR_LCK14 = 1 << 14;
constexpr uint32_t GPIO_LCKR_LCK15 = 1 << 15;
constexpr uint32_t GPIO_LCKR_LCKK  = 1 << 16;

// ----------------------------------------------------------------------------------------
// GPIO alternate function low register (GPIOx_AFRL) (x = A, B, C, F)
//
// AFSELy[3:0]: Alternate function selection for port x pin y (y = 7 to 0)
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_AFRL_AFSEL0 = GENMASK32(3, 0);
constexpr uint32_t GPIO_AFRL_AFSEL1 = GENMASK32(7, 4);
constexpr uint32_t GPIO_AFRL_AFSEL2 = GENMASK32(11, 8);
constexpr uint32_t GPIO_AFRL_AFSEL3 = GENMASK32(15, 12);
constexpr uint32_t GPIO_AFRL_AFSEL4 = GENMASK32(19, 16);
constexpr uint32_t GPIO_AFRL_AFSEL5 = GENMASK32(23, 20);
constexpr uint32_t GPIO_AFRL_AFSEL6 = GENMASK32(27, 24);
constexpr uint32_t GPIO_AFRL_AFSEL7 = GENMASK32(31, 28);

// ----------------------------------------------------------------------------------------
// GPIO alternate function high register (GPIOx_AFRH) (x = A, B, C, F)
//
// AFSELy[3:0]: Alternate function selection for port x, I/O y (y = 15 to 8)
// ----------------------------------------------------------------------------------------
constexpr uint32_t GPIO_AFRH_AFSEL8  = GENMASK32(3, 0);
constexpr uint32_t GPIO_AFRH_AFSEL9  = GENMASK32(7, 4);
constexpr uint32_t GPIO_AFRH_AFSEL10 = GENMASK32(11, 8);
constexpr uint32_t GPIO_AFRH_AFSEL11 = GENMASK32(15, 12);
constexpr uint32_t GPIO_AFRH_AFSEL12 = GENMASK32(19, 16);
constexpr uint32_t GPIO_AFRH_AFSEL13 = GENMASK32(23, 20);
constexpr uint32_t GPIO_AFRH_AFSEL14 = GENMASK32(27, 24);
constexpr uint32_t GPIO_AFRH_AFSEL15 = GENMASK32(31, 28);

// --------------------------------------------------------------------------------------------------------
// GPIO alternate function high and low register (GPIOx_AFRH/GPIOx_AFRL) (x = A, B, C, F) register values
// --------------------------------------------------------------------------------------------------------
constexpr uint32_t GPIO_AFSEL_AF0  = 0b0000;
constexpr uint32_t GPIO_AFSEL_AF1  = 0b0001;
constexpr uint32_t GPIO_AFSEL_AF2  = 0b0010;
constexpr uint32_t GPIO_AFSEL_AF3  = 0b0011;
constexpr uint32_t GPIO_AFSEL_AF4  = 0b0100;
constexpr uint32_t GPIO_AFSEL_AF5  = 0b0101;
constexpr uint32_t GPIO_AFSEL_AF6  = 0b0110;
constexpr uint32_t GPIO_AFSEL_AF7  = 0b0111;
constexpr uint32_t GPIO_AFSEL_AF8  = 0b1000;
constexpr uint32_t GPIO_AFSEL_AF9  = 0b1001;
constexpr uint32_t GPIO_AFSEL_AF10 = 0b1010;
constexpr uint32_t GPIO_AFSEL_AF11 = 0b1011;
constexpr uint32_t GPIO_AFSEL_AF12 = 0b1100;
constexpr uint32_t GPIO_AFSEL_AF13 = 0b1101;
constexpr uint32_t GPIO_AFSEL_AF14 = 0b1110;
constexpr uint32_t GPIO_AFSEL_AF15 = 0b1111;

// --------------------------------------------------------------------------------------------------------
// GPIO port bit reset register (GPIOx_BRR) (x = A, B, C, F)
//
// BRy: Port x reset I/O y (y = 15 to 0)
// --------------------------------------------------------------------------------------------------------
constexpr uint32_t GPIO_BRR_BR0	 = 1 << 0;
constexpr uint32_t GPIO_BRR_BR1	 = 1 << 1;
constexpr uint32_t GPIO_BRR_BR2	 = 1 << 2;
constexpr uint32_t GPIO_BRR_BR3	 = 1 << 3;
constexpr uint32_t GPIO_BRR_BR4	 = 1 << 4;
constexpr uint32_t GPIO_BRR_BR5	 = 1 << 5;
constexpr uint32_t GPIO_BRR_BR6	 = 1 << 6;
constexpr uint32_t GPIO_BRR_BR7	 = 1 << 7;
constexpr uint32_t GPIO_BRR_BR8	 = 1 << 8;
constexpr uint32_t GPIO_BRR_BR9	 = 1 << 9;
constexpr uint32_t GPIO_BRR_BR10 = 1 << 10;
constexpr uint32_t GPIO_BRR_BR11 = 1 << 11;
constexpr uint32_t GPIO_BRR_BR12 = 1 << 12;
constexpr uint32_t GPIO_BRR_BR13 = 1 << 13;
constexpr uint32_t GPIO_BRR_BR14 = 1 << 14;
constexpr uint32_t GPIO_BRR_BR15 = 1 << 15;
