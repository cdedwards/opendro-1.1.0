///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Console port
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
#include <stdio.h>
#include "console.h"
#include "board.h"
#include "gpio.h"
#include "swi.h"

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
console_putc(uint8_t c)
{
    // Wait until debug port is ready and send the character        
    while (!(AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY));
    AT91C_BASE_DBGU->DBGU_THR = c;
}

void
console_write(uint8_t* ptr, int32_t len)
{
    uint32_t n;

    for (n = 0; n < len; n++)
        console_putc(ptr[n]);
}

void
console_isr(void)
{
    uint8_t c;
    while (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY)
    {
        c = AT91C_BASE_DBGU->DBGU_RHR;
        swi_write(SWI_STDIN, &c, sizeof(c));
    }
}

void
console_init(void)
{
    // Assign the DBGU pins to peripheral A
    CONSOLE_PORT->PIO_ASR = (CONSOLE_TX | CONSOLE_RX);
    CONSOLE_PORT->PIO_PDR = (CONSOLE_TX | CONSOLE_RX);

    // Configure DBGU
    AT91F_US_Configure(
        (AT91PS_USART) AT91C_BASE_DBGU,
        MCLK,
        AT91C_US_ASYNC_MODE,
        57600,
        0);

    // Enable the receive ready interupt
    AT91C_BASE_DBGU->DBGU_IER = AT91C_US_RXRDY;

    // Enable transmitter & receivier
    AT91C_BASE_DBGU->DBGU_CR = AT91C_US_RXEN | AT91C_US_TXEN;
}
