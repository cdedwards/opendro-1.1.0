///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// DPU-550 GPIO assignments
//
// Copyright (C) 2008-2011 ShumaTech http://www.shumatech.com/
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////
#ifndef DPU550_GPIO_H_
#define DPU550_GPIO_H_

//
// Scale input pins
//
#define SCALE_IN_PORT       AT91C_BASE_PIOA
#define SCALE0_IN0          AT91C_PIO_PA11
#define SCALE0_IN1          AT91C_PIO_PA12
#define SCALE1_IN0          AT91C_PIO_PA13
#define SCALE1_IN1          AT91C_PIO_PA14
#define SCALE2_IN0          AT91C_PIO_PA24
#define SCALE2_IN1          AT91C_PIO_PA25
#define SCALE3_IN0          AT91C_PIO_PA28
#define SCALE3_IN1          AT91C_PIO_PA29
#define SCALE4_IN0          AT91C_PIO_PA30
#define SCALE4_IN1          AT91C_PIO_PA31

//
// Scale output switch pins
//
#define SWITCH_PORT         AT91C_BASE_PIOA
#define SWITCH_LATCH0       AT91C_PIO_PA26
#define SWITCH_LATCH1       AT91C_PIO_PA27
#define SWITCH_ALL          (DATA0 | DATA1 | DATA2 | DATA3 | DATA4 | DATA5)

//
// Data bus pins
//
#define DATA_PORT           AT91C_BASE_PIOA
#define DATA0               AT91C_PIO_PA16
#define DATA1               AT91C_PIO_PA17
#define DATA2               AT91C_PIO_PA18
#define DATA3               AT91C_PIO_PA19
#define DATA4               AT91C_PIO_PA20
#define DATA5               AT91C_PIO_PA21
#define DATA6               AT91C_PIO_PA22
#define DATA7               AT91C_PIO_PA23
#define DATA_OFFSET         16
#define DATA_ALL            (DATA0 | DATA1 | DATA2 | DATA3 | DATA4 | DATA5 | DATA6 | DATA7)

//
// Display pins
//
#define DISPLAY_LINE_PORT   AT91C_BASE_PIOA
#define DISPLAY_LINE0       AT91C_PIO_PA2
#define DISPLAY_LINE1       AT91C_PIO_PA5
#define DISPLAY_LINE2       AT91C_PIO_PA1
#define DISPLAY_LINE_IND    0

#define DISPLAY_COLUMN_PORT AT91C_BASE_PIOA
#define DISPLAY_COLUMN0     AT91C_PIO_PA6
#define DISPLAY_COLUMN1     AT91C_PIO_PA7
#define DISPLAY_COLUMN2     AT91C_PIO_PA8
#define DISPLAY_COLUMN_ALL  (DISPLAY_COLUMN0 | DISPLAY_COLUMN1 | DISPLAY_COLUMN2)
#define DISPLAY_COLUMN_OFFSET 6

//
// EEPROM I2C pins
//
#define EEPROM_PORT         AT91C_BASE_PIOA
#define EEPROM_SDA          AT91C_PIO_PA3
#define EEPROM_SCL          AT91C_PIO_PA4

//
// Tachometer pins
//
#define TACH_PORT           AT91C_BASE_PIOA
#define TACH0_INPUT         AT91C_PIO_PA15
#define TACH1_INPUT         AT91C_PIO_PA0

//
// Keypad port
//
#define KEYPAD_COLUMN_PORT  AT91C_BASE_PIOA
#define KEYPAD_COLUMN0      DISPLAY_LINE0
#define KEYPAD_COLUMN1      DISPLAY_LINE1
#define KEYPAD_COLUMN2      DATA0
#define KEYPAD_COLUMN3      DATA1
#define KEYPAD_COLUMN4      DATA2
#define KEYPAD_ALL_COLUMNS  (KEYPAD_COLUMN0 | KEYPAD_COLUMN1 | KEYPAD_COLUMN2 | KEYPAD_COLUMN3 | KEYPAD_COLUMN4)

#define KEYPAD_ROW_PORT     AT91C_BASE_PIOA
#define KEYPAD_ROW0         DATA3
#define KEYPAD_ROW1         DATA4
#define KEYPAD_ROW2         DATA5
#define KEYPAD_ROW3         DATA6
#define KEYPAD_ROW4         DATA7
#define KEYPAD_ALL_ROWS     (KEYPAD_ROW0 | KEYPAD_ROW1 | KEYPAD_ROW2 | KEYPAD_ROW3 | KEYPAD_ROW4)

//
// Indicator LED data bits
//
#define IND_LED_INCR        0
#define IND_LED_MM          1
#define IND_LED_ZERO        2
#define IND_LED_SET         3
#define IND_LED_FUNC        4
#define IND_LED_MAX         5

//
// Console pins
//
#define CONSOLE_PORT        AT91C_BASE_PIOA
#define CONSOLE_TX          AT91C_PA10_DTXD
#define CONSOLE_RX          AT91C_PA9_DRXD

//
// PWM pins
//
#define PWM_OUT_PORT        AT91C_BASE_PIOA
#define PWM_OUT1            AT91C_PIO_PA0
#define PWM_OUT_ALL         (PWM_OUT1)

/// \brief  Latch data onto the bus
///
/// \param  data    Data to latch
/// \param  latch   Latch signal to use
void
gpio_latch_data(uint8_t  data,
                pin_t    latch);

#endif // DPU550_GPIO_H_
