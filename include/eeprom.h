///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Atmel AT24C64 EEPROM driver using AT91SAM7S TWI (I2C) peripheral
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
#ifndef EEPROM_H_
#define EEPROM_H_

#include "types.h"

/// Total size of the EEPROM in bytes
#define EEPROM_SIZE     (8 * 1024)

/// \brief  Read bytes from the EEPROM
///
/// \param  address Starting memory address to read
/// \param  buffer  Data buffer to store bytes read
/// \param  length  The total number of bytes to read
///
/// \return The actual number of bytes read or -1 if error.
int32_t
eeprom_read(uint32_t    address,
            void*       buffer,
            uint32_t    length);

/// \brief  Write bytes to the EEPROM
///
/// \param  address Starting memory address to write
/// \param  buffer  Data buffer containing bytes to write
/// \param  length  The total number of bytes to write
///
/// \return The actual number of bytes written or -1 if error.
int32_t
eeprom_write(uint32_t       address,
             const void*    buffer,
             uint32_t       length);

/// \brief  Initialize the EEPROM
void
eeprom_init(void);

#endif // EEPROM_H_
