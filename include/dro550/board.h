///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Board-level hardware definitions
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
#ifndef DRO550_BOARD_H_
#define DRO550_BOARD_H_

#include <stdint.h>

#include "AT91SAM7SE256.h"
#define __inline static inline
#include "lib_AT91SAM7SE256.h"

#if CONFIG_base
#  define OPENDRO_HARDWARE    "dro550"
#elif CONFIG_lcd
#  define OPENDRO_HARDWARE    "dr550l"
#else
#  error "Unknown DRO-550 config"
#endif

#define SCLK                32768       // Hz
#define OSC                 18432000    // Hz
#define OSC_STARTUP         1500        // microseconds
#define PLL_MUL             73
#define PLL_DIV             14
#define PLL_STARTUP         1000        // microseconds
#define CLK_DIV             1           // power of 2 exponent
#define USB_DIV             1           // power of 2 exponent
#define MCLK                (OSC * PLL_MUL / PLL_DIV / (1 << CLK_DIV)) // Hz
#define TIMER_PERIOD        500         // microseconds
#define INVERTING_INPUTS    1           // scale inputs have inverting comparators
#define BEEPER_FREQ         3800        // piezo beeper resonant frequency

// Atmel USB Framework
#define BOARD_USB_ENDPOINTS_MAXPACKETSIZE(i)    ((i == 0) ? 8 : 64)
#define BOARD_USB_UDP
#define BOARD_USB_NUMENDPOINTS                  4
#define BOARD_USB_PULLUP_INTERNAL
#define BOARD_USB_ENDPOINTS_BANKS(i)            (((i == 0) || (i == 3)) ? 1 : 2)
#define BOARD_USB_BMATTRIBUTES                  USBConfigurationDescriptor_SELFPOWERED_NORWAKEUP

/// \brief  Perform an EFC command from SRAM
///
/// \param  cmd     The EFC command to perform
/// \param  pagen   The pagen value specific to the EFC command
void
efc_command(uint8_t cmd,
            uint8_t pagen)
    __attribute__ ((section (".stext")));

void
reset(void);

#endif // DRO550_BOARD_H_
