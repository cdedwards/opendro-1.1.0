///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Scale - this object implements a digital measuring device
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
#ifndef SCALE_H_
#define SCALE_H_

#include <stdio.h>
#include "types.h"
#include "gpio.h"

/// Special scale number to represent that no scale association
#define SCALE_OFF           UINT8_MAX


/// \brief  Get The counts per inch (CPI) of a scale
///
/// \param  scale_num   The scale number (0..SCALE_MAX-1)
///
/// \return The integer counts per inch
uint32_t
scale_get_cpi(uint8_t scale_num);

/// \brief  Get the current counts for a scale
///
/// \param  scale_num   The scale number (0..SCALE_MAX-1)
///
/// \return The integer count
int32_t
scale_get_count(uint8_t scale_num);

/// \brief  Get the mean and variance statistics for a scale's counts
///
/// \param  scale_num   The scale number (0..SCALE_MAX-1)
/// \param  mean        OUT The mean value of the counts
/// \param  variance    OUT The variance of the counts
///
/// \return True on success, false on failure
bool
scale_get_stats(uint8_t scale_num,
                double* mean,
                double* variance);

/// \brief  Send the fast mode signal to any scales not in fast mode
void
scale_send_fast(void);

#if PLATFORM_dro375
/// \brief  Check the fast mode sequence
void
scale_check_fast(void);
#endif

/// \brief  Enter scale setup for a particular machine 
void
scale_setup(void);

/// \brief  Save all scale configuration to non-volatile memory 
void
scale_save(void);

/// \brief  Show the scale state on the console
///
/// \param  out         File pointer to send output to
/// \param  scale_num   The scale number (0..SCALE_MAX-1)
void
scale_show(FILE* out, uint8_t scale_num);

/// \brief  Initialize all instances of the scale object from non-volatile memory
void
scale_init(void);

#endif // SCALE_H_
