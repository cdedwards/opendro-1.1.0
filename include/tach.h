///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Tachometer object
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
#ifndef TACH_H_
#define TACH_H_

#include "types.h"
#include "gpio.h"

/// \brief  Get the tachometer GPIO pin
///
/// \param  tach_num    The tachometer number (0..TACH_MAX-1)
///
/// \return The tachometer GPIO pin
pin_t
tach_pin(uint8_t tach_num);


/// \brief  Get the current tachometer reading
///
/// \param  tach_num    The tachometer number (0..TACH_MAX-1)
///
/// \return The tachometer reading in RPM (0.0 if no reading available)
double
tach_get_reading(uint8_t tach_num);

/// \brief  Initialize the tachometer
void
tach_init(void);

#endif // TACH_H_
