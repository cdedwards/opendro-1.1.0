///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Virtual machine object
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
#ifndef MACHINE_H_
#define MACHINE_H_

#include <stdio.h>
#include "types.h"
#include "axis.h"

/// Machine types
typedef enum machine_type
{
    MACHINE_TYPE_MILL,              ///< Mill machine  
    MACHINE_TYPE_LATHE,             ///< Lathe machine
    MACHINE_TYPE_MAX,               ///< MUST be last!!!    
} machine_type_t;

/// Mill machine axis type to axis number
#define MILL_X_AXIS         0
#define MILL_Y_AXIS         1
#define MILL_Z_AXIS         2
#define MILL_W_AXIS         3
#define MILL_C_AXIS         4

/// Lathe machine axis type to axis number
#define LATHE_X_AXIS        0
#define LATHE_Z1_AXIS       1
#define LATHE_Z2_AXIS       2
#define LATHE_W_AXIS        3
#define LATHE_C_AXIS        4

/// Number of axis types
#define MACHINE_AXIS_TYPES  5
#if (MACHINE_AXIS_TYPES < AXIS_MAX)
#error "Must have at least as many machine axis types as real axes"
#endif

/// \brief  Convert a decimal floating point value to its constituent
///         integral and fractional parts.
///
/// \param  decimal     The decimal floating point number to convert
/// \param  maxdenom    The maximum denominator to use
/// \param  integral    OUT The integral part of the number
/// \param  numerator   OUT The numerator of the fractional component
/// \param  denominator OUT The denominator of the fractional component
void
machine_dec2frac(double decimal,
                 int32_t maxdenom,
                 int32_t* integral,
                 int32_t* numerator,
                 int32_t* denominator);

/// \brief  Get the axis number assigned to a display line
///
/// \param  line_num        The display line (0..DISPLAY_LINE_MAX-1)
///
/// \return Axis number line is assigned to
uint8_t
machine_axis_by_line(uint8_t line_num);

/// \brief  Get the axis number by its text label
///
/// \param  label           String pointer of axis label
///
/// \return Axis number corresponding to label or AXIS_MAX if none
uint8_t
machine_axis_by_label(const char* label);

/// \brief  Get the axis label by its number
///
/// \param  axis_num        The axis number (0..AXIS_MAX-1)
///
/// \return Label corresponding to the axis number
const char*
machine_axis_get_label(uint8_t axis_num);
                      
/// \brief  Get the type of the machine
///
/// \return The machine type
machine_type_t
machine_get_type(void);

/// \brief  Get the current zero mode of the machine
///
/// \return The zero mode (absolute or incremental)
axis_zero_t
machine_get_mode(void);

/// \brief  Get the current measurement units of the machine
///
/// \return The current units (inch or mm)
axis_units_t
machine_get_units(void);

/// \brief  Get the current measurement units of the machine
///
/// \param  units           The units to set (inch or mm)
void
machine_set_units(axis_units_t units);

/// \brief  Get the active machine number
///
/// \return The machine number (0..MACHINE_MAX-1)
uint8_t
machine_get_active(void);

/// \brief  Set the active machine number
///
/// \param  machine_num     The machine number (0..MACHINE_MAX-1)
void
machine_set_active(uint8_t machine_num);

/// \brief  Set the tachometer to display on a line
///
/// \param  line_num        The display line (0..DISPLAY_LINE_MAX-1)
///                         (>=DISPLAY_LINE_MAX for none)
/// \param  surface         Set to true if tachometer is displayed in surface rate
///                         or to false for RPM
void
machine_set_tach(uint8_t    line_num,
                 bool       surface);

/// \brief  Get the line the tachometer is displayed on
///
/// \param  line_num        OUT The display line (0..DISPLAY_LINE_MAX-1)
///                         (>=DISPLAY_LINE_MAX for none)
/// \param  surface         OUT Set to true if tachometer is displayed in surface rate
///                         or to false for RPM
void
machine_get_tach(uint8_t*   line_num,
                 bool*      surface);

/// \brief  Set the axis and line the feed rate is calculated on.
///
/// \param  line_num        The display line (0..DISPLAY_LINE_MAX-1)
///                         (>=DISPLAY_LINE_MAX for none)
/// \param  axis_num        The axis of measurement (0..AXIS_MAX-1)
void
machine_set_feed(uint8_t    line_num,
                 uint8_t    axis_num);

/// \brief  Get the axis and line the feed rate is calculated on.
///
/// \param  line_num        OUT The display line (0..DISPLAY_LINE_MAX-1)
///                         (>=DISPLAY_LINE_MAX for none)
/// \param  axis_num        OUT The axis of measurement (0..AXIS_MAX-1)
void
machine_get_feed(uint8_t*   line_num,
                 uint8_t*   axis_num);

/// \brief  Set the zero reference for all axes on this machine
///
/// \param  zero            The zero reference for all axes
void
machine_set_zero(axis_zero_t    zero);

/// \brief  Get the zero reference for all axes on this machine
///
/// \return The zero reference for all axes
axis_zero_t
machine_get_zero(void);

/// \brief  Set the prompt enable
///
/// \param  enable          Prompt enable (TRUE to enable)
void
machine_set_prompt(bool enable);

/// \brief  Get the prompt enable
///
/// \return The prompt enable (TRUE if prompts are enable)
bool
machine_get_prompt(void);

/// \brief  Set all machines prompts without saving
///
/// \param  enable          Prompt enable (TRUE to enable)
void
machine_all_prompt(bool enable);

/// \brief  Toggle between the display modes for this machine
void
machine_display_toggle(void);

/// \brief  Turn beeper for current machine on
void
machine_beeper_on(void);

/// \brief  Turn beeper for current machine off
void
machine_beeper_off(void);

/// \brief  Main entry point to process the machine
void
machine_process(void);

/// \brief  Turn off the machine
void
machine_off(void);

/// \brief  Turn on the machine
void
machine_on(void);

/// \brief  Stop the machine
void
machine_stop(void);

/// \brief  Called when machine is idle to handle machine background processing
///
/// \param update       true to update machine readings, false to not
void
machine_idle(bool update);

/// \brief  Called when machine is busy again after calling machine_idle
void
machine_busy(void);

/// \brief  Save all machine configuration
void
machine_save(void);

/// \brief  Enter machine setup
void
machine_setup();

/// \brief  Machine show
///
/// \param  out             File pointer to send output to
void
machine_show(FILE* out);

/// \brief  Initialized all machine instances
void
machine_init(void);

#endif // MACHINE_H_
