///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Standard input and output
//
// Copyright (C) 2009-2011 ShumaTech http://www.shumatech.com/
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
#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "types.h"

/// \brief  Output a character to the console
/// \param  c   character to output
void
console_putc(uint8_t c);

/// \brief  Write a buffer to the console
/// \param  ptr     Buffer pointer
/// \param  len     Length of buffer
void
console_write(uint8_t* ptr, int32_t len);

/// \brief  Service the console ISR
void
console_isr(void);

/// \brief  Initialize the console
void
console_init(void);

#endif // CONSOLE_H_
