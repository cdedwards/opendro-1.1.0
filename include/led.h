///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Multiplexed seven segment LED display object
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
#ifndef LED_H_
#define LED_H_

#include "display_dev.h"

/// LED column that is safe to park at
#define LED_COLUMN_NONE             (LED_COLUMN_MAX + 1)

/// LED display device driver
extern const display_dev_t led_display_dev;

/// Platform-specific helper functions
void
led_platform_line(uint8_t line, uint8_t data);

void
led_platform_column(uint8_t column);

void
led_platform_init(void);

/// \brief  Stop processing the LED display
void
led_stop(void);

/// \brief  Start processing the LED display
void
led_start(void);

#endif // LED_H_
