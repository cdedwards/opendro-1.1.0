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
#ifndef DRO550_GPIO_H_
#define DRO550_GPIO_H_

//
// Scale input pins
//
#define SCALE_IN_PORT       AT91C_BASE_PIOA
#define SCALE0_IN0          AT91C_PIO_PA16
#define SCALE0_IN1          AT91C_PIO_PA27
#define SCALE1_IN0          AT91C_PIO_PA17
#define SCALE1_IN1          AT91C_PIO_PA28
#define SCALE2_IN0          AT91C_PIO_PA18
#define SCALE2_IN1          AT91C_PIO_PA29
#define SCALE3_IN0          AT91C_PIO_PA19
#define SCALE3_IN1          AT91C_PIO_PA30
#define SCALE4_IN0          AT91C_PIO_PA20
#define SCALE4_IN1          AT91C_PIO_PA31
#define SCALE_IN_ALL        (SCALE0_IN0 | SCALE0_IN1 | SCALE1_IN0 | SCALE1_IN1 | \
                             SCALE2_IN0 | SCALE2_IN1 | SCALE3_IN0 | SCALE3_IN1 | \
                             SCALE4_IN0 | SCALE4_IN1)

//
// Scale output pins
//
#define SCALE_OUT_PORT      AT91C_BASE_PIOB
#define SCALE0_OUT0         AT91C_PIO_PB22
#define SCALE0_OUT1         AT91C_PIO_PB27
#define SCALE1_OUT0         AT91C_PIO_PB23
#define SCALE1_OUT1         AT91C_PIO_PB28
#define SCALE2_OUT0         AT91C_PIO_PB24
#define SCALE2_OUT1         AT91C_PIO_PB29
#define SCALE3_OUT0         AT91C_PIO_PB25
#define SCALE3_OUT1         AT91C_PIO_PB30
#define SCALE4_OUT0         AT91C_PIO_PB26
#define SCALE4_OUT1         AT91C_PIO_PB31
#define SCALE_OUT_ALL       (SCALE0_OUT0 | SCALE0_OUT1 | SCALE1_OUT0 | SCALE1_OUT1 | \
                             SCALE2_OUT0 | SCALE2_OUT1 | SCALE3_OUT0 | SCALE3_OUT1 | \
                             SCALE4_OUT0 | SCALE4_OUT1)

//
// Display pins
//
#define DISPLAY_COLUMN_PORT AT91C_BASE_PIOB
#define DISPLAY_COLUMN0     AT91C_PIO_PB1
#define DISPLAY_COLUMN1     AT91C_PIO_PB2
#define DISPLAY_COLUMN2     AT91C_PIO_PB3
#define DISPLAY_COLUMN3     AT91C_PIO_PB4
#define DISPLAY_COLUMN4     AT91C_PIO_PB5
#define DISPLAY_COLUMN5     AT91C_PIO_PB6
#define DISPLAY_COLUMN6     AT91C_PIO_PB7
#define DISPLAY_COLUMN7     AT91C_PIO_PB8
#define DISPLAY_COLUMN_ALL  (DISPLAY_COLUMN0 | DISPLAY_COLUMN1 | DISPLAY_COLUMN2 | DISPLAY_COLUMN3 | \
                             DISPLAY_COLUMN4 | DISPLAY_COLUMN5 | DISPLAY_COLUMN6 | DISPLAY_COLUMN7)
#define DISPLAY_COLUMN_CLK  AT91C_PIO_PB16

#define DISPLAY_LINE_PORT   AT91C_BASE_PIOA
#define DISPLAY_LINE_LE     AT91C_PIO_PA11
#define DISPLAY_LINE_SO     AT91C_PIO_PA12
#define DISPLAY_LINE_SI     AT91C_PIO_PA13
#define DISPLAY_LINE_CLK    AT91C_PIO_PA14
#define DISPLAY_LINE_OE     AT91C_PIO_PA15
#define DISPLAY_LINE_IND    0

//
// Indicator LED data bits
//
#define IND_LED_INCR        4
#define IND_LED_MM          3
#define IND_LED_ZERO        2
#define IND_LED_SET         1
#define IND_LED_FUNC        0
#define IND_LED_MAX         5

//
// EEPROM I2C pins
//
#define EEPROM_PORT         AT91C_BASE_PIOA
#define EEPROM_SDA          AT91C_PIO_PA3
#define EEPROM_SCL          AT91C_PIO_PA4

//
// Tachometer pins
//
#define TACH_PORT           AT91C_BASE_PIOB
#define TACH0_INPUT         AT91C_PIO_PB0
#define TACH1_INPUT         AT91C_PIO_PB15

//
// Keypad port
//
#define KEYPAD_COLUMN_PORT  AT91C_BASE_PIOB
#define KEYPAD_COLUMN0      AT91C_PIO_PB17
#define KEYPAD_COLUMN1      AT91C_PIO_PB18
#define KEYPAD_COLUMN2      AT91C_PIO_PB19
#define KEYPAD_COLUMN3      AT91C_PIO_PB20
#define KEYPAD_COLUMN4      AT91C_PIO_PB21
#define KEYPAD_ALL_COLUMNS  (KEYPAD_COLUMN0 | KEYPAD_COLUMN1 | KEYPAD_COLUMN2 | KEYPAD_COLUMN3 | KEYPAD_COLUMN4)

#define KEYPAD_ROW_PORT     AT91C_BASE_PIOB
#define KEYPAD_ROW0         AT91C_PIO_PB9
#define KEYPAD_ROW1         AT91C_PIO_PB10
#define KEYPAD_ROW2         AT91C_PIO_PB11
#define KEYPAD_ROW3         AT91C_PIO_PB12
#define KEYPAD_ROW4         AT91C_PIO_PB13
#define KEYPAD_ALL_ROWS     (KEYPAD_ROW0 | KEYPAD_ROW1 | KEYPAD_ROW2 | KEYPAD_ROW3 | KEYPAD_ROW4)

#if CONFIG_lcd
#define KEYPAD_LCD_PORT     EXP1_PORT
#define KEYPAD_LCD_COLUMN   EXP1_1
#endif

//
// Console pins
//
#define CONSOLE_PORT        AT91C_BASE_PIOA
#define CONSOLE_TX          AT91C_PIO_PA10
#define CONSOLE_RX          AT91C_PIO_PA9

//
// Piezo beeper pin
//
#define BEEPER_PORT         AT91C_BASE_PIOB
#define BEEPER_DRIVE        AT91C_PIO_PB14

//
// PWM pins
//
#define PWM_OUT_PORT        AT91C_BASE_PIOA
#define PWM_OUT1            AT91C_PIO_PA0
#define PWM_OUT2            AT91C_PIO_PA1
#define PWM_OUT3            AT91C_PIO_PA2
#define PWM_OUT_ALL         (PWM_OUT1 | PWM_OUT2 | PWM_OUT3)

//
// LCD UART
//
#define LCD_UART_PORT       AT91C_BASE_PIOA
#define LCD_UART_TX         AT91C_PIO_PA6
#define LCD_UART_RX         AT91C_PIO_PA5
#define LCD_UART_ALL        (LCD_UART_TX | LCD_UART_RX | LCD_UART_IO)

//
// EXP UART
//
#define EXP_UART_PORT       AT91C_BASE_PIOA
#define EXP_UART_TX         AT91C_PIO_PA22
#define EXP_UART_RX         AT91C_PIO_PA21
#define EXP_UART_ALL        (EXP_UART_TX | EXP_UART_RX)

//
// EXP1 Bus
//
#define EXP1_PORT            AT91C_BASE_PIOC
#define EXP1_1               AT91C_PIO_PC8
#define EXP1_2               AT91C_PIO_PC9
#define EXP1_3               AT91C_PIO_PC10
#define EXP1_4               AT91C_PIO_PC11
#define EXP1_5               AT91C_PIO_PC12
#define EXP1_6               AT91C_PIO_PC13
#define EXP1_7               AT91C_PIO_PC14
#define EXP1_8               AT91C_PIO_PC15
#define EXP1_9               AT91C_PIO_PC19
#define EXP1_ALL             (EXP1_1 | EXP1_2 | EXP1_3 | EXP1_4 | EXP1_5 | EXP1_6 | EXP1_7 | EXP1_8 | EXP1_9)

//
// EXP2 Bus
//
#define EXP2_PORT            AT91C_BASE_PIOA
#define EXP2_1               AT91C_PIO_PA23
#define EXP2_2               AT91C_PIO_PA24
#define EXP2_3               AT91C_PIO_PA25
#define EXP2_4               AT91C_PIO_PA26
#define EXP2_5               AT91C_PIO_PA7
#define EXP2_ALL             (EXP2_1 | EXP2_2 | EXP2_3 | EXP2_4 | EXP2_5)

//
// USB
//
#define USB_PORT             AT91C_BASE_PIOA
#define USB_VUSB             AT91C_PIO_PA8
#define USB_ALL              (USB_VUSB)

//
// GDM12864 LCD Bus
//
#define GDM12864_PORT        AT91C_BASE_PIOC
#define GDM12864_DB0         AT91C_PIO_PC0
#define GDM12864_DB1         AT91C_PIO_PC1
#define GDM12864_DB2         AT91C_PIO_PC2
#define GDM12864_DB3         AT91C_PIO_PC3
#define GDM12864_DB4         AT91C_PIO_PC4
#define GDM12864_DB5         AT91C_PIO_PC5
#define GDM12864_DB6         AT91C_PIO_PC6
#define GDM12864_DB7         AT91C_PIO_PC7
#define GDM12864_DB_OFFSET   0
#define GDM12864_DB_ALL      (GDM12864_DB0 | GDM12864_DB1 | GDM12864_DB2 | GDM12864_DB3 | GDM12864_DB4 | GDM12864_DB5 | GDM12864_DB6 | GDM12864_DB7)
#define GDM12864_CS2         AT91C_PIO_PC17
#define GDM12864_CS1         AT91C_PIO_PC18
#define GDM12864_CS_ALL      (GDM12864_CS1 | GDM12864_CS2)
#define GDM12864_RES         AT91C_PIO_PC20
#define GDM12864_R_W         AT91C_PIO_PC21
#define GDM12864_D_I         AT91C_PIO_PC22
#define GDM12864_E           AT91C_PIO_PC23
#define GDM12864_CTRL_ALL    (GDM12864_CS1 | GDM12864_CS2 | GDM12864_RES | GDM12864_D_I | GDM12864_R_W | GDM12864_E)
#define GDM12864_BKLIGHT     AT91C_PIO_PC16

//
// SD Memory Card
//
#define SD_CARD_PORT         EXP1_PORT
#define SD_CARD_CS           EXP1_2
#define SD_CARD_WP           EXP1_3
#define SD_CARD_CMD          EXP1_4
#define SD_CARD_CD           EXP1_5
#define SD_CARD_SCLK         EXP1_6
#define SD_CARD_IND          EXP1_7
#define SD_CARD_D0           EXP1_8
#define SD_CARD_IRQ          EXP1_9

/// \brief  Get the pin associated to a scale input
///
/// \param  scale   Scale to return input for (0..SCALE_MAX-1)
/// \param  input   Scale input number (0..SCALE_IN_MAX-1)
///
/// \return The pin associated with the scale input
pin_t
gpio_scale_out(uint8_t scale,
               uint8_t input);

/// \brief  Read data from the LCD
///
/// \param  cs      LCD chip select number
///
/// \return The data from the scale
uint8_t
gpio_lcd_read_data(uint8_t cs);

/// \brief  Read status from the LCD
///
/// \param  cs      LCD chip select number
///
/// \return The status from the scale
uint8_t
gpio_lcd_read_status(uint8_t cs);

/// \brief  Write data to the LCD
///
/// \param  cs      LCD chip select number
/// \param  data    Data to write to the LCD
void
gpio_lcd_write_data(uint8_t cs,
                    uint8_t data);

/// \brief  Write an instruction to the LCD
///
/// \param  cs      LCD chip select number
/// \param  intr    Instruction to write to the LCD
void
gpio_lcd_write_instr(uint8_t cs,
                     uint8_t instr);

#endif // DRO550_GPIO_H_
