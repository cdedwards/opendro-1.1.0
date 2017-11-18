///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Atmel AT24CXX EEPROM driver using AT91SAM7S TWI (I2C) peripheral
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
#include <string.h>
#include "eeprom.h"
#include "board.h"
#include "timer.h"
#include "gpio.h"
#include "assert.h"
#include "trace.h"
#include "irq.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

#define TWI_CLOCK_SPEED     100000      ///< I2C clock speed
#define TWI_EEPROM_ADDR     0x50        ///< I2C slave address for AT24CXX
#define TWI_PAGE_SIZE       32          ///< Page size for writes
#define TWI_CLOCK_DIV       1           ///< I2C clock divisor

/// EEPROM transfer structure for communicating between the application
/// and the ISR.  This MUST be volatile to assure coherency.
static volatile struct
{
    bool        complete;       ///< Flag to indicate the tranfser is complete
    uint8_t     *buffer;        ///< Buffer holding data to read or write
    uint32_t    length;         ///< Length of the data in the buffer
} eeprom_xfer;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

// Poll the EEPROM to check if it is ready for another transfer.  This returns
// true if it is and false if it is not.
static bool
eeprom_ready(void)
{
    uint8_t dummy;
    
    // Setup the transfer
    eeprom_xfer.complete = false;
    eeprom_xfer.length = 1;
    eeprom_xfer.buffer = &dummy;
    
    // Configure the EEPROM slave address
    AT91C_BASE_TWI->TWI_MMR = (TWI_EEPROM_ADDR << 16) | AT91C_TWI_MREAD;

    // Enable the interrupts and start
    AT91C_BASE_TWI->TWI_IER = AT91C_TWI_RXRDY | AT91C_TWI_NACK;
    AT91C_BASE_TWI->TWI_CR = AT91C_TWI_START | AT91C_TWI_STOP;

    // Wait for transfer complete
    while (!eeprom_xfer.complete);

    return (eeprom_xfer.length == 0);
}

// TWI peripheral interrupt service routine.  This checks each of the status
// bits and performs the appropriate actions for each.
static void
eeprom_isr(void)
{
    uint32_t status = (AT91C_BASE_TWI->TWI_SR & AT91C_BASE_TWI->TWI_IMR);
    
    // The slave NACK'ed us so the whole thing is off
    if (status & AT91C_TWI_NACK)
    {
        AT91C_BASE_TWI->TWI_IDR = 0xffffffff;
        eeprom_xfer.complete = true;
    }
    // A byte was received from the slave
    else if (status & AT91C_TWI_RXRDY)
    {
        *eeprom_xfer.buffer++ = AT91C_BASE_TWI->TWI_RHR;  
        eeprom_xfer.length--;
    
        // Switch to the transfer complete interrupt if done
        if (eeprom_xfer.length == 0)
        {
            AT91C_BASE_TWI->TWI_IDR = 0xffffffff;
            AT91C_BASE_TWI->TWI_IER = AT91C_TWI_TXCOMP;
        }
        // Stop the transfer if the next byte is last
        else if (eeprom_xfer.length == 1)
        {
            AT91C_BASE_TWI->TWI_CR = AT91C_TWI_STOP;
        }
    }
    // The TWI peripheral is ready to transmit another byte
    else if (status & AT91C_TWI_TXRDY)
    {
        // Stop and switch to the transfer complete interrupt if done
        if (eeprom_xfer.length == 0)
        {
            AT91C_BASE_TWI->TWI_CR = AT91C_TWI_STOP;
            AT91C_BASE_TWI->TWI_IDR = 0xffffffff;
            AT91C_BASE_TWI->TWI_IER = AT91C_TWI_TXCOMP;
        }
        // Else transmit the next byte in the buffer
        else
        {
            AT91C_BASE_TWI->TWI_THR = *eeprom_xfer.buffer++;
            eeprom_xfer.length--;
        }
    }
    // The transfer is complete so signal the application
    else if (status & AT91C_TWI_TXCOMP)
    {
        AT91C_BASE_TWI->TWI_IDR = AT91C_TWI_TXCOMP;
        eeprom_xfer.complete = true;
    }
}

#ifdef DEBUG
static void
eeprom_hexdump(const uint8_t* buffer,
               uint32_t length)
{
    while(length-- > 0)
        TRACE_DEBUG_WP(" %02x", *buffer++);
    TRACE_DEBUG_WP("\n");
}
#endif

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

int32_t
eeprom_read(uint32_t    address,
            void*       buffer,
            uint32_t    length)
{
    TRACE_DEBUG("%s @ 0x%x len %d\n", __FUNCTION__, address, length);

    if (address >= EEPROM_SIZE)
        return -1;
    if (address + length > EEPROM_SIZE)
        length = EEPROM_SIZE - address;
    
    // Setup the transfer
    eeprom_xfer.complete = false;
    eeprom_xfer.length = length;
    eeprom_xfer.buffer = buffer;
    
    // Configure the EEPROM slave and data addresses
    AT91C_BASE_TWI->TWI_MMR = (TWI_EEPROM_ADDR << 16) | AT91C_TWI_IADRSZ_2_BYTE | AT91C_TWI_MREAD;
    AT91C_BASE_TWI->TWI_IADR = address;

    // Set stop if transfer is a single byte
    if (length == 1)
        AT91C_BASE_TWI->TWI_CR = AT91C_TWI_STOP;
    
    // Enable the interrupts and start
    AT91C_BASE_TWI->TWI_IER = AT91C_TWI_RXRDY | AT91C_TWI_NACK;
    AT91C_BASE_TWI->TWI_CR = AT91C_TWI_START;

    // Wait for transfer complete
    while (!eeprom_xfer.complete);
    
    // Warn if the read was short
    if (eeprom_xfer.length != 0)
        TRACE_WARNING("EEPROM read from %x short %d bytes\n", address, eeprom_xfer.length);

#ifdef DEBUG
    eeprom_hexdump(buffer, length);
#endif
    
    // Return number of bytes actually read
    return (length - eeprom_xfer.length);   
}

int32_t
eeprom_write(uint32_t       address,
             const void*    buffer,
             uint32_t       length)
{
    uint32_t        byte_count;
    uint32_t        xfer_len;
    const uint8_t*  byte_ptr = buffer;
    
    TRACE_DEBUG("%s @ 0x%x len %d\n", __FUNCTION__, address, length);
#ifdef DEBUG
    eeprom_hexdump(buffer, length);
#endif

    if (address >= EEPROM_SIZE)
        return -1;
    if (address + length > EEPROM_SIZE)
        length = EEPROM_SIZE - address;

    for (byte_count = 0; byte_count < length; byte_count += xfer_len)
    {
        // Calculate the size of the transfer staying within the EEPROM page size
        xfer_len = MIN(length - byte_count, TWI_PAGE_SIZE - (address % TWI_PAGE_SIZE));
                        
        // Setup the transfer minus the first byte
        eeprom_xfer.complete = false;
        eeprom_xfer.length = xfer_len - 1;
        eeprom_xfer.buffer = (uint8_t *) byte_ptr + 1;
        
        // Configure the EEPROM slave and data addresses
        AT91C_BASE_TWI->TWI_MMR = (TWI_EEPROM_ADDR << 16) | AT91C_TWI_IADRSZ_2_BYTE;
        AT91C_BASE_TWI->TWI_IADR = address;

        // Transfer the first byte
        AT91C_BASE_TWI->TWI_THR = *byte_ptr;
        
        // Enable the interrupts and start
        AT91C_BASE_TWI->TWI_IER = AT91C_TWI_TXRDY | AT91C_TWI_NACK;
        AT91C_BASE_TWI->TWI_CR = AT91C_TWI_START;
        
        // Wait for transfer complete
        while (!eeprom_xfer.complete);

        // Wait for the EEPROM write to finish
        while (!eeprom_ready());
        
        // Add the transfer length to the address and byte pointer     
        address += xfer_len;
        byte_ptr += xfer_len;
        
        // Stop if the write was short
        if (eeprom_xfer.length != 0)
        {
            TRACE_WARNING("EEPROM write to %x short %d bytes\n", address, eeprom_xfer.length);
            break;
        }
    }
    
    // Return number of bytes actually written
    return (byte_count - eeprom_xfer.length);   
}

void
eeprom_init(void)
{
    uint32_t period;
    
    // Assign the EEPROM pins to peripheral A, open-drain, with pull-ups
    EEPROM_PORT->PIO_ASR   = (EEPROM_SDA | EEPROM_SCL);
    EEPROM_PORT->PIO_MDER  = (EEPROM_SDA | EEPROM_SCL);
    EEPROM_PORT->PIO_PPUER = (EEPROM_SDA | EEPROM_SCL);
    EEPROM_PORT->PIO_PDR   = (EEPROM_SDA | EEPROM_SCL);

    // Enable the TWI peripheral clock
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_TWI);
    
    // Reset the TWI
    AT91C_BASE_TWI->TWI_CR = AT91C_TWI_SWRST;

    // Set up the clock waveform generator
    period = ((MCLK / (2 * TWI_CLOCK_SPEED)) - 3) / (1 << TWI_CLOCK_DIV);
    AT91C_BASE_TWI->TWI_CWGR = (TWI_CLOCK_DIV << 16) | (period << 8) | period;

    // Set master mode
    AT91C_BASE_TWI->TWI_CR = AT91C_TWI_MSEN;

    // Register the interrupt handler
    irq_register(AT91C_ID_TWI, IRQ_PRIORITY_HIGHEST, IRQ_TYPE_LEVEL, eeprom_isr);
}
