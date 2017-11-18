///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Microprocessor GPIO operations
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
#ifndef GPIO_H_
#define GPIO_H_

#include "types.h"
#include "board.h"

/// Bit array representing the pins on a GPIO port (i.e PA12 = bit 12)
typedef uint32_t pin_t;

/// Data type for the GPIO port
typedef AT91PS_PIO port_t;

#if PLATFORM_dro375
#  include "dro375/gpio.h"
#elif PLATFORM_dpu550
#  include "dpu550/gpio.h"
#elif PLATFORM_dro550
#  include "dro550/gpio.h"
#endif

/// Configure pins as inputs
#define GPIO_INPUT(port, pin)      {(port)->PIO_ODR = (pin);\
                                   (port)->PIO_PER = (pin);}

/// Configure pins as outputs
#define GPIO_OUTPUT(port, pin)     {(port)->PIO_OER = (pin);\
                                   (port)->PIO_PER = (pin);}

/// Configure pin to peripheral A
#define GPIO_PERIPH_A(port, pin)   {(port)->PIO_ASR = (pin);\
                                   (port)->PIO_PDR = (pin);}

/// Configure pin to peripheral B
#define GPIO_PERIPH_B(port, pin)   {(port)->PIO_BSR = (pin);\
                                   (port)->PIO_PDR = (pin);}

/// Configure pins as interrupt on change
#define GPIO_INTERRUPT(port, pin)  {(port)->PIO_IER = (pin);}

/// Read interrupt on change status
#define GPIO_INT_STATUS(port)      ((port)->PIO_ISR)

/// Configure pin pull-up
#define GPIO_PULLUP(port, pin)     {(port)->PIO_PPUER = (pin);}
#define GPIO_NOPULLUP(port, pin)   {(port)->PIO_PPUDR = (pin);}

/// Configure pin open drain
#define GPIO_OPENDRAIN(port, pin)  {(port)->PIO_MDER = (pin);}

/// Drive pins high
#define GPIO_HIGH(port, pin)       {(port)->PIO_SODR = (pin);}

/// Drive pins low
#define GPIO_LOW(port, pin)        {(port)->PIO_CODR = (pin);}

/// True if pin is high
#define GPIO_IS_HIGH(port, pin)    ((port)->PIO_PDSR & (pin))

/// True if pin is low
#define GPIO_IS_LOW(port, pin)     (!((port)->PIO_PDSR & (pin)))

/// Set parallel data
#define GPIO_DATA_SET(port, mask, data) \
                                  {(port)->PIO_OWDR = 0xffffffff;\
                                   (port)->PIO_OWER = (mask);\
                                   (port)->PIO_ODSR = (data);}

/// Get parallel data
#define GPIO_DATA_GET(port, mask)  ((port)->PIO_PDSR & mask)

/// \brief  Initialize the microprocessor GPIO interface
void
gpio_init(void);

#endif // GPIO_H_
