///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Graphic LCD device driver for GDM12864 (2 x KS0108B)
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

#include "board.h"
#include "gpio.h"
#include "timer.h"
#include "gdm12864.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

#define KS0108B_STATUS_BUSY     0x80
#define KS0108B_STATUS_OFF      0x40
#define KS0108B_STATUS_RESET    0x10

#define KS0108B_INSTR_DISPLAY   0x3e
#define KS0108B_INSTR_X_ADDR    0xb8
#define KS0108B_INSTR_Y_ADDR    0x40
#define KS0108B_INSTR_START     0xc0

typedef enum gdm12864_state
{
    GDM12864_STATE_INIT,
    GDM12864_STATE_OFF,
    GDM12864_STATE_ON,
} gdm12864_state_t;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

// Map chip index to the chip select pin
static const pin_t gdm12864_chip[KS0108B_CHIPS] = {
    GDM12864_CS1,
    GDM12864_CS2,
};

static gdm12864_state_t gdm12864_state = GDM12864_STATE_INIT;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

static inline void
gdm12864_delay(void)
{
    __asm__ (
        "\tnop\n"
        "\tnop\n"
        "\tnop\n"
        "\tnop\n"
        "\tnop\n"
        "\tnop\n"
        "\tnop\n"
        "\tnop\n"
    );
}

static uint8_t
gdm12864_read_cycle(uint8_t chip)
{
    uint8_t data;

    GPIO_HIGH(GDM12864_PORT, GDM12864_R_W);
    GPIO_LOW(GDM12864_PORT, gdm12864_chip[chip]);
    gdm12864_delay();
    GPIO_HIGH(GDM12864_PORT, GDM12864_E);
    gdm12864_delay();
    gdm12864_delay();
    data = (GPIO_DATA_GET(GDM12864_PORT, GDM12864_DB_ALL) >> GDM12864_DB_OFFSET);
    GPIO_LOW(GDM12864_PORT, GDM12864_E);
    GPIO_HIGH(GDM12864_PORT, gdm12864_chip[chip]);
    return data;
}

#if 0 // Reserved for debug
static uint8_t
gdm12864_read_data(uint8_t chip)
{
    GPIO_HIGH(GDM12864_PORT, GDM12864_D_I);
    return gdm12864_read_cycle(chip);
}
#endif

static uint8_t
gdm12864_read_status(uint8_t chip)
{
    GPIO_LOW(GDM12864_PORT, GDM12864_D_I);
    return gdm12864_read_cycle(chip);
}

static void
gdm12864_wait_ready(uint8_t chip)
{
    while(gdm12864_read_status(chip) & KS0108B_STATUS_BUSY);
}

static void
gdm12864_write_cycle(uint8_t chip, uint8_t data)
{
    GPIO_LOW(GDM12864_PORT, GDM12864_R_W);
    GPIO_LOW(GDM12864_PORT, gdm12864_chip[chip]);
    gdm12864_delay();
    GPIO_HIGH(GDM12864_PORT, GDM12864_E);
    GPIO_DATA_SET(GDM12864_PORT, GDM12864_DB_ALL, data << GDM12864_DB_OFFSET);
    GPIO_OUTPUT(GDM12864_PORT, GDM12864_DB_ALL);
    gdm12864_delay();
    gdm12864_delay();
    GPIO_LOW(GDM12864_PORT, GDM12864_E);
    GPIO_HIGH(GDM12864_PORT, gdm12864_chip[chip]);
    GPIO_INPUT(GDM12864_PORT, GDM12864_DB_ALL);
}

static void
gdm12864_write_data(uint8_t chip, uint8_t data)
{
    gdm12864_wait_ready(chip);
    GPIO_HIGH(GDM12864_PORT, GDM12864_D_I);
    return gdm12864_write_cycle(chip, data);
}

static void
gdm12864_write_instr(uint8_t chip, uint8_t instr)
{
    gdm12864_wait_ready(chip);
    GPIO_LOW(GDM12864_PORT, GDM12864_D_I);
    return gdm12864_write_cycle(chip, instr);
}

static void
gdm12864_clear(void)
{
    uint32_t x;
    uint32_t y;
    uint8_t chip = 0;

    if (gdm12864_state == GDM12864_STATE_INIT)
        return;

    for (y = 0; y < GDM12864_DISPLAY_HEIGHT; y += BITS_PER_BYTE)
    {
        for (x = 0; x < GDM12864_DISPLAY_WIDTH; x++)
        {
            if (x % KS0108B_WIDTH == 0)
            {
                chip = x / KS0108B_WIDTH;
                gdm12864_write_instr(chip, KS0108B_INSTR_X_ADDR | (y / BITS_PER_BYTE));
                gdm12864_write_instr(chip, KS0108B_INSTR_Y_ADDR);
            }

            gdm12864_write_data(chip, 0);
        }
    }
}

static void
gdm12864_on(void)
{
    uint8_t chip;

    if (gdm12864_state != GDM12864_STATE_OFF)
        return;

    gdm12864_state = GDM12864_STATE_ON;

    // Turn the backlight on
    GPIO_LOW(GDM12864_PORT, GDM12864_BKLIGHT);

    // Turn the display on
    for (chip = 0; chip < KS0108B_CHIPS; chip++)
        gdm12864_write_instr(chip, KS0108B_INSTR_DISPLAY | 0x1);
}

static void
gdm12864_off(void)
{
    uint8_t chip;

    if (gdm12864_state != GDM12864_STATE_ON)
        return;

    gdm12864_state = GDM12864_STATE_OFF;

    // Turn the backlight off
    GPIO_HIGH(GDM12864_PORT, GDM12864_BKLIGHT);

    // Turn the display off
    for (chip = 0; chip < KS0108B_CHIPS; chip++)
        gdm12864_write_instr(chip, KS0108B_INSTR_DISPLAY);
}

static void
gdm12864_update(uint8_t* buffer)
{
    uint32_t x;
    uint32_t y;
    uint8_t row;
    uint8_t byte;
    uint8_t mask;
    uint8_t chip = 0;

    if (gdm12864_state != GDM12864_STATE_ON)
        return;

    for (y = 0; y < GDM12864_DISPLAY_HEIGHT; y += BITS_PER_BYTE)
    {
        for (x = 0; x < GDM12864_DISPLAY_WIDTH; x++)
        {
            if (x % KS0108B_WIDTH == 0)
            {
                chip = x / KS0108B_WIDTH;
                gdm12864_write_instr(chip, KS0108B_INSTR_X_ADDR | (y / BITS_PER_BYTE));
                gdm12864_write_instr(chip, KS0108B_INSTR_Y_ADDR);
            }

            byte = 0;
            mask = 1 << (BITS_PER_BYTE - (x % BITS_PER_BYTE) - 1);
            for (row = 0; row < 8; row++)
            {
                if (buffer[((y + row) * GDM12864_DISPLAY_WIDTH / BITS_PER_BYTE) +
                            (x / BITS_PER_BYTE)]
                    & mask)
                {
                    byte |= 1 << row;
                }
            }
            gdm12864_write_data(chip, byte);
        }
    }

    // The module sometimes spontaneously turns off so make sure it is on.
    for (chip = 0; chip < KS0108B_CHIPS; chip++)
        gdm12864_write_instr(chip, KS0108B_INSTR_DISPLAY | 0x1);
}

static bool
gdm12864_init(void)
{
    // Set CSx, RES, D/I, and R/W control signals high output
    GPIO_HIGH(GDM12864_PORT, GDM12864_CS_ALL | GDM12864_RES | GDM12864_D_I | GDM12864_R_W);
    GPIO_NOPULLUP(GDM12864_PORT, GDM12864_CS_ALL | GDM12864_RES | GDM12864_D_I | GDM12864_R_W);
    GPIO_OUTPUT(GDM12864_PORT, GDM12864_CS_ALL | GDM12864_RES | GDM12864_D_I | GDM12864_R_W);

    // Set E low output
    GPIO_LOW(GDM12864_PORT, GDM12864_E);
    GPIO_NOPULLUP(GDM12864_PORT, GDM12864_E);
    GPIO_OUTPUT(GDM12864_PORT, GDM12864_E);

    // Turn the backlight on
    GPIO_LOW(GDM12864_PORT, GDM12864_BKLIGHT);
    GPIO_OPENDRAIN(GDM12864_PORT, GDM12864_BKLIGHT);
    GPIO_NOPULLUP(GDM12864_PORT, GDM12864_BKLIGHT);
    GPIO_OUTPUT(GDM12864_PORT, GDM12864_BKLIGHT);

    // Reset module
    GPIO_LOW(GDM12864_PORT, GDM12864_RES);
    GPIO_NOPULLUP(GDM12864_PORT, GDM12864_RES);
    GPIO_OUTPUT(GDM12864_PORT, GDM12864_RES);
    timer_delay();

    // Make sure the display responds and reports as reset
    if ((gdm12864_read_status(0) & 0x5f) != KS0108B_STATUS_RESET)
        return false;

    // Release the reset
    GPIO_HIGH(GDM12864_PORT, GDM12864_RES);

    gdm12864_state = GDM12864_STATE_ON;

    // Clear the display
    gdm12864_clear();

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// G L O B A L   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

const lcd_dev_t lcd_dev = {
    .clear  = gdm12864_clear,
    .on     = gdm12864_on,
    .off    = gdm12864_off,
    .update = gdm12864_update,
    .init   = gdm12864_init,
};
