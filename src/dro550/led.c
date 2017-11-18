///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// DRO-550 LED functions
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
#include "drolim.h"
#include "gpio.h"
#include "led.h"
#include "assert.h"
#include "trace.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

static inline void
led_delay(void)
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

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
led_platform_line(uint8_t line, uint8_t data)
{
    static uint8_t line_state[LED_LINE_MAX];
    ASSERT(line < LED_LINE_MAX);

    line_state[line] = data;

    // Send the data out ONLY on the last line
    if (line == LED_LINE_MAX - 1)
    {
        // Send the lines bottom line first
        do
        {
            // Wait for transmit data empty
            while (!(AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TDRE));

            // Transmit the row data
            AT91C_BASE_SPI->SPI_TDR = line_state[line];
        } while (line-- > 0);

        // Wait for all of the data to go out
        while (!(AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TXEMPTY));

        // Latch the row data in
        GPIO_HIGH(DISPLAY_LINE_PORT, DISPLAY_LINE_LE);
        led_delay();
        GPIO_LOW(DISPLAY_LINE_PORT, DISPLAY_LINE_LE);
    }
}

void
led_platform_column(uint8_t column)
{
    static pin_t gpio_map[LED_COLUMN_NONE + 1] =
    {
        DISPLAY_COLUMN0,
        DISPLAY_COLUMN1,
        DISPLAY_COLUMN2,
        DISPLAY_COLUMN3,
        DISPLAY_COLUMN4,
        DISPLAY_COLUMN5,
        DISPLAY_COLUMN6,  // LED_COLUMN_INDICATORS
        0,                // LED_COLUMN_NONE
    };

    ASSERT(column <= LED_COLUMN_NONE);

    // For no display column, disable the row outputs first
    if (column == LED_COLUMN_NONE)
        GPIO_HIGH(DISPLAY_LINE_PORT, DISPLAY_LINE_OE);

    // Assert the new column value
    GPIO_DATA_SET(DISPLAY_COLUMN_PORT, DISPLAY_COLUMN_ALL, gpio_map[column]);

    // Latch in the new column value
    GPIO_HIGH(DISPLAY_COLUMN_PORT, DISPLAY_COLUMN_CLK);
    led_delay();
    GPIO_LOW(DISPLAY_COLUMN_PORT, DISPLAY_COLUMN_CLK);

    // For a valid column, make sure the row outputs are enabled
    if (column != LED_COLUMN_NONE)
        GPIO_LOW(DISPLAY_LINE_PORT, DISPLAY_LINE_OE);
}

void
led_platform_init(void)
{
    // Set display row output enable high
    GPIO_HIGH(DISPLAY_LINE_PORT, DISPLAY_LINE_OE);
    GPIO_OUTPUT(DISPLAY_LINE_PORT, DISPLAY_LINE_OE);

    // Set display row latch enable low
    GPIO_LOW(DISPLAY_LINE_PORT, DISPLAY_LINE_LE);
    GPIO_OUTPUT(DISPLAY_LINE_PORT, DISPLAY_LINE_LE);

    // Set display column clock low
    GPIO_LOW(DISPLAY_COLUMN_PORT, DISPLAY_COLUMN_CLK);
    GPIO_OUTPUT(DISPLAY_COLUMN_PORT, DISPLAY_COLUMN_CLK);

    // Set display column outputs low
    GPIO_LOW(DISPLAY_COLUMN_PORT, DISPLAY_COLUMN_ALL);
    GPIO_OUTPUT(DISPLAY_COLUMN_PORT, DISPLAY_COLUMN_ALL);

    // Enable the SPI peripheral clock
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_SPI);

    // Assign the SPI pins to peripheral A
    GPIO_PERIPH_A(DISPLAY_LINE_PORT, DISPLAY_LINE_SI | DISPLAY_LINE_CLK | DISPLAY_LINE_SO);

    // Configure the SPI port to master without mode fault detection
    AT91C_BASE_SPI->SPI_MR = AT91C_SPI_MSTR | AT91C_SPI_MODFDIS;

    // Configure the SPI clock polarity and phase, the bits per transfer to 8, and
    // the SPI clock rate to MCLK / 24 (2 MHz)
    AT91C_BASE_SPI->SPI_CSR[0] = AT91C_SPI_NCPHA | (24 << 8);

    // Enable SPI to send and receive
    AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN;
}
