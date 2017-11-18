///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Axis of measurement object
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
#ifndef AXIS_H_
#define AXIS_H_

#include <stdio.h>
#include "types.h"

/// Special axis number to represent that no association exists
/// to the axis
#define AXIS_OFF        UINT8_MAX

/// Standard unit conversions
#define INCH_TO_MM      (25.4)
#define MM_TO_INCH      (1/25.4)

/// Axis reading type
typedef enum axis_zero
{
    AXIS_ZERO_ABS,          ///< Absolute reading
    AXIS_ZERO_INC,          ///< Incremental reading
} axis_zero_t;

/// Axis measurement units
typedef enum axis_units
{
    AXIS_UNITS_INCH,        ///< Units in inches
    AXIS_UNITS_MM,          ///< Units in millimeters
    AXIS_UNITS_MAX,         ///< MUST be last!!!  
} axis_units_t;

/// Fractional precision
#define PRECISION_FRACTION      UINT8_MAX

/// \brief  Set the active workspace used for absolute and incremental
/// reference points.
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
/// \param  workspace   The workspace number (0 - WORKSPACE_MAX)
void 
axis_set_workspace(uint8_t axis_num,
                   uint8_t workspace);

/// \brief Set the amount to factor the associated scale into the axis  
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
void
axis_set_scale_factor(uint8_t   axis_num,
                      double    scale_factor);

/// \brief  Get the current scale factor for this axis
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
///
/// \return The current scale factor
double
axis_get_scale_factor(uint8_t axis_num);

/// \brief  Set the amount of tool offset to apply to this axis
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
/// \param  tool_offset The tool offset amount
/// \param  units       The units for the offset
void
axis_set_tool_offset(uint8_t        axis_num,
                     double         tool_offset,
                     axis_units_t   units);

/// \brief  
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
/// \param  units       The units to return the tool offset in
///
/// \return The tool offset in inches
double
axis_get_tool_offset(uint8_t        axis_num,
                     axis_units_t   units);

/// \brief  Set a new absolute zero for the current workspace referenced from
/// the current absolute zero
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
/// \param  absolute    New absolute zero  referenced from the current absolute zero
/// \param  units       The units for the absolute zero
void
axis_set_absolute(uint8_t       axis_num,
                  double        absolute,
                  axis_units_t  units);

/// \brief  Set a new incremental zero for the current workspace referenced from
/// the current absolute zero.
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
/// \param  incremental New incremental zero referenced from the current absolute zero
/// \param  units       The units for the absolute zero
void
axis_set_incremental(uint8_t        axis_num,
                     double         incremental,
                     axis_units_t   units);

/// \brief  Get the absolute zero for the current workspace referenced from the scale zero
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
/// \param  units       The units to return the zero in
///
/// \return The absolute zero
double
axis_get_absolute(uint8_t       axis_num,
                  axis_units_t  units);

/// \brief  Get the incremental zero for the current workspace referenced from the absolute zero
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
/// \param  units       The units to return the zero in
///
/// \return The incremental zero
double
axis_get_incremental(uint8_t        axis_num,
                     axis_units_t   units);

/// \brief  Get the final reading for the axis after applying all operations
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
/// \param  units       The units for the reading
/// \param  zero        The zero reference for the reading
///
/// \return 
double
axis_get_reading(uint8_t        axis_num,
                 axis_units_t   units,
                 axis_zero_t    zero);

/// \brief  Get the scale associated with this axis
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
///
/// \return 
uint8_t
axis_get_scale(uint8_t axis_num);

/// \brief  Get the axis reading precision
///
/// \param  axis_num    The axis number (0..AXIS_MAX-1)
/// \param  units       The units for the precision
///
/// \return Reading precision in number of half digits or
///         PRECISION_FRACTION if fractional
uint8_t
axis_get_precision(uint8_t      axis_num,
                   axis_units_t units);

/// \brief  Set axis vectoring for an axis
///
/// \param  axis_num        The axis number (0..AXIS_MAX-1)
/// \param  vector_axis     The axis to vector (0..AXIS_MAX-1)
/// \param  vector_factor   The amount to vector into the axis
void
axis_set_vector(uint8_t axis_num,
                uint8_t vector_axis,
                double  vector_factor);

/// \brief  Convert value from one axis units to another
///
/// \param  value           Value to convert
/// \param  from_units      The units to convert from
/// \param  to_units        The units to convert to
///
/// \return The converted value
double
axis_convert_units(double value,
                   axis_units_t from_units,
                   axis_units_t to_units);
                
/// \brief  Save all axis configuration to non-volatile memory 
void
axis_save(void);

/// \brief  Enter axis setup for a particular machine 
void
axis_setup(void);

/// \brief  Show axis state
///
/// \param  out             File pointer to send output to
/// \param  axis_num        The axis number (0..AXIS_MAX-1)
void
axis_show(FILE* out, uint8_t axis_num);

/// \brief  Initialize all instances of the axis object from non-volatile memory
void
axis_init(void);

#endif // AXIS_H_
